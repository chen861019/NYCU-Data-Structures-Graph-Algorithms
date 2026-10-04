"""Import coursework sources and embedded report figures into a separate repo."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import xml.etree.ElementTree as ET
from pathlib import Path
from zipfile import ZipFile


ASSIGNMENTS = [
    ("I", "assignment01-date", ["Programming_Assignment_I.c"]),
    ("II", "assignment02-sorting", ["p2_1.c", "P2_2.c", "p2_3r.c", "p2_3s.c"]),
    ("III", "assignment03-maze", ["Programming_Assignment_III.c"]),
    ("IV", "assignment04-sparse-matrix", ["Programming_Assignment_IV.c"]),
    ("V", "assignment05-expression", ["Programming_Assignment_V.c"]),
    ("VI", "assignment06-segment-tree", ["Programming_Assignment_VI.c"]),
    ("VII", "assignment07-huffman", ["7_14.c"]),
    ("VIII", "assignment08-mst", ["8_5.c", "8_txt.c"]),
    ("IX", "assignment09-shortest-path", ["9_4.c", "9_txt.c"]),
]


def decode_source(data: bytes) -> tuple[str, str]:
    for encoding in ("utf-8-sig", "cp950", "cp1252"):
        try:
            return data.decode(encoding), encoding
        except UnicodeDecodeError:
            continue
    raise UnicodeError("Source file could not be decoded losslessly")


def import_text(source: Path, destination: Path, manifest: list, root: Path) -> None:
    original = source.read_bytes()
    text, encoding = decode_source(original)
    text = text.replace("\r\n", "\n")
    text = re.sub(
        r"(?im)^\s*//\s*(?:student\s*(?:number|id)|email|學號|電子郵件)\s*[:：].*\n?",
        "",
        text,
    )
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_text(text, encoding="utf-8", newline="\n")
    manifest.append({
        "source": source.relative_to(root).as_posix(),
        "destination": destination.as_posix(),
        "source_encoding": encoding,
        "source_sha256": hashlib.sha256(original).hexdigest(),
        "imported_sha256": hashlib.sha256(destination.read_bytes()).hexdigest(),
    })


def report_text(archive: ZipFile) -> str:
    namespace = {"w": "http://schemas.openxmlformats.org/wordprocessingml/2006/main"}
    root = ET.fromstring(archive.read("word/document.xml"))
    return "\n".join(
        "".join(node.text or "" for node in paragraph.findall(".//w:t", namespace))
        for paragraph in root.findall(".//w:p", namespace)
    )


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    args = parser.parse_args()
    root = args.source.resolve()
    local = Path(".local")
    local.mkdir(exist_ok=True)
    manifest: list[dict] = []
    counts = {}
    for roman, name, primary in ASSIGNMENTS:
        original_dir = root / f"Programming_Assignment_{roman}"
        destination = Path("assignments") / name
        for source in sorted(original_dir.rglob("*.c")):
            section = "src" if source.name in primary else "archive"
            relative = source.relative_to(original_dir)
            import_text(source, destination / section / relative, manifest, root)

        reports = list(original_dir.rglob(f"Programming_Assignment_{roman}.docx"))
        if len(reports) != 1:
            raise ValueError(f"Expected one report for Assignment {roman}")
        with ZipFile(reports[0]) as archive:
            (local / f"{name}-report.txt").write_text(report_text(archive), encoding="utf-8")
            media = [entry for entry in archive.namelist() if entry.startswith("word/media/")]
            media.sort(key=lambda entry: int(re.search(r"\d+", Path(entry).stem).group()))
            for index, entry in enumerate(media, 1):
                assets = destination / "assets"
                assets.mkdir(exist_ok=True)
                extension = Path(entry).suffix.lower().replace(".jpeg", ".jpg")
                (assets / f"report-{index:02d}{extension}").write_bytes(archive.read(entry))
            counts[name] = len(media)

        from pypdf import PdfReader

        task = next(original_dir.rglob(f"p{ASSIGNMENTS.index((roman, name, primary)) + 1}.pdf"))
        text = "\n".join(page.extract_text() or "" for page in PdfReader(task).pages)
        (local / f"{name}-task.txt").write_text(text, encoding="utf-8")

    data_files = {
        "II": ["random_numbers.txt"],
        "VII": ["input.txt", "output.txt"],
        "VIII": ["graph.txt", "graph.dot", "mst.dot", "graph.png", "mst.png"],
        "IX": ["graph.txt"],
    }
    names = {roman: name for roman, name, _ in ASSIGNMENTS}
    for roman, files in data_files.items():
        original_dir = root / f"Programming_Assignment_{roman}"
        for filename in files:
            output = Path("assignments") / names[roman] / "examples" / filename
            output.parent.mkdir(parents=True, exist_ok=True)
            if output.suffix in {".txt", ".dot"}:
                import_text(original_dir / filename, output, manifest, root)
            else:
                shutil.copyfile(original_dir / filename, output)
    for dirname in ("5X6", "6X12", "20X40"):
        original_dir = root / "Programming_Assignment_IX" / dirname
        destination = Path("assignments") / names["IX"] / "examples" / dirname.lower()
        destination.mkdir(parents=True, exist_ok=True)
        for source in original_dir.iterdir():
            if source.suffix == ".dot":
                import_text(source, destination / source.name, manifest, root)
            elif source.suffix == ".png":
                shutil.copyfile(source, destination / source.name)
    for filename in ("0a-gcd.c", "segment_tree.c"):
        import_text(root / filename, Path("extras") / filename, manifest, root)
    (Path("docs")).mkdir(exist_ok=True)
    Path("docs/source-manifest.json").write_text(
        json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
    )
    print(json.dumps({"text_files": len(manifest), "report_figures": counts}, indent=2))


if __name__ == "__main__":
    main()
