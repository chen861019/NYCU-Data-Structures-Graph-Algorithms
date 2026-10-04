"""Create local contact sheets for reviewing report figures before publication."""

from pathlib import Path

from PIL import Image, ImageDraw, ImageOps


def main() -> None:
    output = Path(".local/contact-sheets")
    output.mkdir(parents=True, exist_ok=True)
    for assignment in sorted(Path("assignments").iterdir()):
        assets = sorted((assignment / "assets").glob("report-*"))
        for offset in range(0, len(assets), 16):
            batch = assets[offset:offset + 16]
            sheet = Image.new("RGB", (1600, ((len(batch) + 3) // 4) * 300), "#e5e7eb")
            draw = ImageDraw.Draw(sheet)
            for index, path in enumerate(batch):
                x, y = (index % 4) * 400, (index // 4) * 300
                if path.suffix == ".svg":
                    draw.text((x + 10, y + 60), "SVG vector figure", fill="black")
                else:
                    with Image.open(path) as figure:
                        thumb = ImageOps.contain(figure.convert("RGB"), (390, 268))
                    sheet.paste(thumb, (x + 5, y + 24))
                draw.text((x + 7, y + 5), path.name, fill="black")
            target = output / f"{assignment.name}-{offset // 16 + 1}.jpg"
            sheet.save(target, quality=90)
            print(target)


if __name__ == "__main__":
    main()
