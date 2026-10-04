"""Generate source inventories and report galleries from reviewed metadata."""

from __future__ import annotations

import json
import re
from pathlib import Path
from urllib.parse import quote


COURSE = [
    ("assignment01-date", "I", "日期運算", {"Programming_Assignment_I.c": "日期表示、星期、天數差與前後位移"}, "operations.txt 每行為一個獨立執行範例；主程式每次只處理一行。"),
    ("assignment02-sorting", "II", "五種排序", {"p2_1.c": "基礎排序比較", "P2_2.c": "主要 DEBUG／計時與排序檢查版本", "p2_3s.c": "資料檔產生器", "p2_3r.c": "資料檔讀取與排序比較"}, "random_numbers.txt 是原課程保存的文字資料，不是本次 DEBUG 20 筆測例。檔案型程式依工作目錄讀寫此檔。"),
    ("assignment03-maze", "III", "BFS 迷宮", {"Programming_Assignment_III.c": "整合矩陣輸入、BFS 與路徑回溯"}, "maze.in 是本次新增的 4 x 5 可達測例；unreachable.in 是 1 x 3 不可達測例。欄號 1-based，每列以 0 結束。"),
    ("assignment04-sparse-matrix", "IV", "稀疏矩陣", {"Programming_Assignment_IV.c": "鏈結串列矩陣、加法、乘法與轉置"}, "matrices.in 是本次新增的兩個 2 x 2 矩陣。欄號遞增且 1-based，每列以 0 結束。"),
    ("assignment05-expression", "V", "運算式計算器", {"Programming_Assignment_V.c": "雙 stack 求值、基本四則與變數指派"}, "expressions.txt 是本次新增的多行求值與變數範例。讀到 EOF 結束。"),
    ("assignment06-segment-tree", "VI", "區間最大值", {"Programming_Assignment_VI.c": "遞迴 Max Segment Tree 與 Brute Force 比較"}, "queries.in 是本次新增的查詢範例。先 n 再 m，區間為 0-based inclusive；陣列內容由程式隨機產生。"),
    ("assignment07-huffman", "VII", "Huffman", {"7_14.c": "頻率、Min-Heap、編碼、bit packing 與同程序解碼"}, "input.txt／output.txt 是原課程歷史文字檔。一般模式會重新產生 input.txt，請在獨立工作目錄執行，不要覆寫此處。"),
    ("assignment08-mst", "VIII", "Kruskal MST", {"8_5.c": "Kruskal 與 DOT 輸出", "8_txt.c": "隨機圖資料產生器"}, "small-graph.txt 是本次固定 5 頂點測例，MST 總權重 6。graph.txt、DOT 與 PNG 為原資料夾保留的歷史範例，不以新測例權重解讀。"),
    ("assignment09-shortest-path", "IX", "Dijkstra", {"9_4.c": "Adjacency List、線性選點 Dijkstra 與所有 source 的 DOT", "9_txt.c": "隨機圖資料產生器"}, "small-graph.txt 是本次固定測例；graph.txt 與 5x6／6x12／20x40 為歷史資料。每個子資料夾另有 DOT／PNG 索引。"),
]


def figure_caption(name: str, number: int, suffix: str) -> str:
    if name.startswith("assignment01"):
        return "外部日期工具的歷史對照截圖（非自行開發介面）" if number % 2 == 0 else "日期程式的歷史操作與結果"
    if name.startswith("assignment02"):
        return {1: "DEBUG 20 筆的排序前後與檢查", 2: "10,000 筆歷史排序時間", 3: "50,000 筆歷史排序時間", 4: "100,000 筆歷史排序時間"}[number]
    if name.startswith("assignment03"):
        return "迷宮輸入、障礙配置與 BFS 路徑歷史結果"
    if name.startswith("assignment04"):
        return {7: "矩陣加法歷史結果", 8: "矩陣乘法歷史結果", 15: "4 x 5 矩陣加法歷史結果", 16: "維度不符的歷史提示", 22: "4 x 4 矩陣乘法歷史結果", 29: "維度不符的歷史案例", 30: "維度不符的歷史案例", 33: "單位矩陣乘法歷史結果"}.get(number, "矩陣輸入、串列、轉置或運算結果的原報告畫面")
    if name.startswith("assignment05"):
        return "一般執行模式的歷史結果" if number == 23 else "運算式、stack 追蹤與結果的原報告畫面"
    if name.startswith("assignment06"):
        return "互動查詢與最大值核對" if number <= 2 else "不同資料量／查詢量的歷史時間（比較限制見 README）"
    if name.startswith("assignment07"):
        return {1: "DEBUG 輸入與符號頻率", 2: "Huffman 碼表", 3: "Huffman Tree 示意", 10: "原報告的外部比較畫面（非本次驗證）"}.get(number, "檔案大小、壓縮／解壓縮與比率的歷史結果")
    if name.startswith("assignment08"):
        return "原始圖、MST 或 DOT／操作的歷史結果" if number <= 16 else "大型圖資料與整輪時間的歷史結果（單位秒）"
    if suffix == ".svg":
        return "歷史圖形的向量版本（鄰近 PNG 為顯示用版本）"
    if number >= 25:
        return "大型圖的所有 source 執行與計時歷史結果"
    return "加權圖、各 source 路徑或 shortest-path tree 的歷史結果"


def source_functions(path: Path) -> list[str]:
    text = path.read_text(encoding="utf-8")
    names = re.findall(r"^[ \t]*[A-Za-z_][A-Za-z0-9_ *\t]*?\b(\w+)\s*\([^;{}]*\)\s*\{", text, re.MULTILINE)
    return list(dict.fromkeys(name for name in names if name not in {"if", "for", "while", "switch"}))


def main() -> None:
    verification = json.loads(Path("docs/verification.json").read_text(encoding="utf-8"))
    builds = {item["source"]: item for item in verification["builds"]}
    overview = ["# 作業對照與閱讀地圖", "", "[課程首頁](../README.md)", "", "主版本依原始碼、作業主題及報告線索選定，不是只依最後修改時間猜測。舊版本保留原檔名，僅將九個作業外層資料夾統一成 assignment01 至 assignment09。", "", "| 作業 | 主題 | C 程式 | 報告圖 | 詳細入口 |", "| --- | --- | --- | --- | --- |"]
    for name, roman, title, primary, examples_text in COURSE:
        folder = Path("assignments") / name
        sources = sorted(folder.rglob("*.c"))
        assets = sorted((folder / "assets").iterdir())
        overview.append(f"| {roman} | {title} | {len(sources)} | {len(assets)} | [說明](../assignments/{name}/README.md) |")
        inventory = [f"# Assignment {roman}：程式逐檔索引", "", "[作業說明](README.md) · [圖片索引](RESULTS.md)", "", "主要程式與原課程演進／測試版本分開保存。下列編譯狀態引用本次 verification.json；有 main 不表示已通過所有需求，也不建議把所有舊版連結成同一程式。", "", "| 原始碼 | 角色 | GCC 7.4.0 編譯 |", "| --- | --- | --- |"]
        for source in sources:
            relative = source.relative_to(folder).as_posix()
            build = builds[source.as_posix()]
            status = "成功（有診斷／警告）" if build["diagnostic"] and build["exit_code"] == 0 else "成功" if build["exit_code"] == 0 else "失敗：保留原始草稿"
            role = primary.get(source.name) if relative.startswith("src/") else "原始演進／拆分實驗版本；詳細入口見下方函式索引"
            inventory.append(f"| [{relative}]({quote(relative)}) | {role} | {status} |")
        inventory.extend(["", "## 函式入口", "", "以下是由 C 函式定義擷取的閱讀索引，不是對所有函式功能的自動保證。主版本的資料結構與演算法說明以作業 README 為準。", ""])
        for source in sources:
            relative = source.relative_to(folder).as_posix()
            inventory.extend([f"### {relative}", "", "、".join(f"`{function}`" for function in source_functions(source)) or "未擷取到可列出的函式定義，請直接閱讀原碼。", ""])
        (folder / "FILES.md").write_text("\n".join(inventory), encoding="utf-8", newline="\n")
        gallery = [f"# Assignment {roman}：完整報告圖片", "", "[作業說明](README.md) · [程式索引](FILES.md)", "", f"本頁收錄原 Word 報告內的 {len(assets)} 張圖片。依媒體檔編號排列，該順序不保證等同 Word 的閱讀順序；不是本次驗證工具的新截圖。圖中文字、時間與測試資料均屬歷史紀錄，解讀請配合作業 README。", "", "PNG 可直接檢視；SVG 另提供向量連結，不將向量與 PNG 重複版本算成額外獨立測試。", ""]
        for asset in assets:
            number = int(re.search(r"\d+", asset.stem).group())
            caption = figure_caption(name, number, asset.suffix)
            gallery.extend([f"## 圖 {number:02d}：{caption}", "", f"[向量圖檔]({asset.relative_to(folder).as_posix()})" if asset.suffix == ".svg" else f"![圖 {number:02d}：{caption}]({asset.relative_to(folder).as_posix()})", ""])
        (folder / "RESULTS.md").write_text("\n".join(gallery), encoding="utf-8", newline="\n")
        for section in ("src", "archive"):
            destination = folder / section
            if destination.exists():
                details = "主要版本與資料產生器。操作與限制見作業主說明。" if section == "src" else "原始課程演進與實驗版本，保留供對照，不視為穩定或完成所有規格。編譯失敗的草稿也不刪除或偽裝成功。"
                lines = [f"# Assignment {roman}：{section}", "", details, "", "[作業主說明](../README.md) · [逐檔角色、編譯與函式索引](../FILES.md)", ""]
                destination.joinpath("README.md").write_text("\n".join(lines), encoding="utf-8", newline="\n")
        (folder / "examples").mkdir(exist_ok=True)
        example_lines = [f"# Assignment {roman}：輸入與圖形範例", "", "[作業說明](../README.md)", "", examples_text, "", "請把工作輸入複製到 build/ 的獨立資料夾後執行；有寫檔功能的程式不要直接在本範例資料夾執行。", "", "## 檔案", ""]
        for example in sorted((folder / "examples").iterdir()):
            if example.name == "README.md":
                continue
            target = example.name + "/README.md" if example.is_dir() else example.name
            example_lines.append(f"- [{example.name}]({quote(target)})")
            if example.is_dir():
                lines = [f"# 歷史圖形範例：{example.name}", "", "[作業說明](../../README.md) · [範例總索引](../README.md)", "", "檔名 original_graph 為原圖，shortest_path_tree_X 為 source X 的最短路徑樹。DOT 是 Graphviz 描述檔，PNG 是原課程保存的渲染圖；不保證與上層 graph.txt 或新測例為同一次生成資料。", ""]
                for graph in sorted(example.iterdir()):
                    if graph.suffix == ".dot":
                        lines.append(f"- [{graph.name}]({quote(graph.name)})")
                for graph in sorted(example.glob("*.png")):
                    lines.extend(["", f"## {graph.stem}", "", f"![{graph.stem}]({quote(graph.name)})"])
                example.joinpath("README.md").write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
        (folder / "examples/README.md").write_text("\n".join(example_lines) + "\n", encoding="utf-8", newline="\n")
    overview.extend(["", "## 學習銜接", "", "日期操作練習資料模型與邊界；排序引入複雜度與計時；BFS 使用 queue；稀疏矩陣練習 pointer 與串列；計算器用 stack 管理優先序；Segment Tree 預處理重複查詢；Huffman 與 Kruskal 展示不同貪婪策略；Dijkstra 則將距離與路徑記錄結合。", "", "## 來源與公開處理", "", "原始課程資料只讀，不覆寫。匯入 C／文字／DOT 的原相對路徑、編碼與前後 SHA-256 記錄在 [source-manifest.json](source-manifest.json)。新增加的小型輸入範例與 Markdown 不冒充歷史來源。", "", "Word 報告用於抽圖與交叉確認；PDF 題目／報告／講義不直接公開，亦未上傳 EXE、XLSX、備份及原垃圾桶中的 7 份 C 草稿。對第三方的 GCD 參考程式保留原作者 D. J. Guan 註記。", "", "只移除原始碼標頭中的學號／電子郵件，轉 UTF-8 與 LF；沒有為了展示而補上缺失演算法。圖形時間屬歷史結果；新編譯與代表性測例見 [VERIFICATION.md](VERIFICATION.md)。", ""])
    Path("docs/ASSIGNMENTS.md").write_text("\n".join(overview), encoding="utf-8", newline="\n")
    print("Generated 9 assignment inventories and galleries, with section/example indexes.")


if __name__ == "__main__":
    main()
