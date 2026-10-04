"""Check public files, provenance hashes, image integrity and Markdown links."""

from __future__ import annotations

import hashlib
import json
import re
import xml.etree.ElementTree as ET
from pathlib import Path
from urllib.parse import unquote, urlsplit

from PIL import Image


def main() -> None:
    root = Path.cwd().resolve()
    files = [path for path in root.rglob("*") if path.is_file() and not any(part in {".git", ".local", "build", "__pycache__"} for part in path.relative_to(root).parts)]
    errors = []
    markdown_links = 0
    for path in files:
        relative = path.relative_to(root).as_posix()
        if path.suffix.lower() in {".exe", ".docx", ".pdf", ".xlsx", ".bkp", ".o", ".obj"}:
            errors.append(f"Unexpected public artifact: {relative}")
        if path.suffix == ".c":
            content = path.read_text(encoding="utf-8")
            if re.search(r"(?im)^\s*//\s*(?:student\s*(?:number|id)|email|學號|電子郵件)\s*[:：]", content):
                errors.append(f"Personal header remains: {relative}")
            if "\ufffd" in content:
                errors.append(f"Replacement character in source: {relative}")
        if path.suffix == ".md":
            for target in re.findall(r"!?\[[^\]\n]*\]\(([^\n)]+)\)", path.read_text(encoding="utf-8")):
                target = target.strip().strip("<>")
                if not target or target.startswith("#") or urlsplit(target).scheme:
                    continue
                resolved = (path.parent / unquote(target.split("#", 1)[0])).resolve()
                markdown_links += 1
                if not resolved.is_relative_to(root) or not resolved.exists():
                    errors.append(f"Missing/escaping link: {relative} -> {target}")
        if path.suffix.lower() in {".png", ".jpg", ".jpeg"}:
            try:
                with Image.open(path) as picture:
                    picture.verify()
            except Exception as error:
                errors.append(f"Invalid image {relative}: {error}")
        if path.suffix == ".svg":
            try:
                document = ET.parse(path)
                for node in document.iter():
                    tag = node.tag.rsplit("}", 1)[-1].lower()
                    if tag in {"script", "foreignobject"}:
                        errors.append(f"Active SVG element: {relative}")
                    for key, value in node.attrib.items():
                        if key.rsplit("}", 1)[-1].lower().startswith("on") or "javascript:" in value.lower():
                            errors.append(f"Active SVG attribute: {relative}")
                        if key.rsplit("}", 1)[-1] == "href" and value.startswith(("http:", "https:", "file:")):
                            errors.append(f"External SVG reference: {relative}")
            except ET.ParseError as error:
                errors.append(f"Invalid SVG {relative}: {error}")
        if path.suffix == ".py":
            compile(path.read_text(encoding="utf-8"), relative, "exec")
    manifest = json.loads((root / "docs/source-manifest.json").read_text(encoding="utf-8"))
    for item in manifest:
        imported = root / item["destination"]
        if hashlib.sha256(imported.read_bytes()).hexdigest() != item["imported_sha256"]:
            errors.append(f"Provenance hash mismatch: {item['destination']}")
    verification = json.loads((root / "docs/verification.json").read_text(encoding="utf-8"))
    primary = [item for item in verification["builds"] if "/src/" in item["source"]]
    print(json.dumps({
        "public_files": len(files),
        "markdown_files": sum(path.suffix == ".md" for path in files),
        "local_links_checked": markdown_links,
        "c_sources": sum(path.suffix == ".c" for path in files),
        "report_figures": sum(path.parent.name == "assets" for path in files),
        "image_files": sum(path.suffix in {".png", ".jpg", ".svg"} for path in files),
        "provenance_text_files": len(manifest),
        "primary_compiled": sum(item["exit_code"] == 0 for item in primary),
        "primary_sources": len(primary),
        "verification_summary": verification["summary"],
        "errors": errors,
    }, ensure_ascii=False, indent=2))
    if errors:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
