"""Compile imported C programs and check representative coursework behavior."""

from __future__ import annotations

import argparse
import datetime as dt
import json
import os
import re
import shutil
import subprocess
import tempfile
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--cc", default="gcc")
    args = parser.parse_args()
    compiler = shutil.which(args.cc) or str(Path(args.cc).resolve())
    environment = dict(os.environ)
    environment["PATH"] = str(Path(compiler).parent) + os.pathsep + environment.get("PATH", "")
    version = subprocess.run([compiler, "--version"], capture_output=True, env=environment, check=True).stdout.decode(errors="replace").splitlines()[0]
    flags = ["-std=gnu11", "-D_POSIX_C_SOURCE=200809L", "-O0", "-Wall", "-Wextra"]
    builds, checks = [], []
    repository = Path.cwd()
    sources = sorted(list(Path("assignments").rglob("*.c")) + list(Path("extras").rglob("*.c")))
    with tempfile.TemporaryDirectory(prefix="nycu-dsga-") as directory:
        temporary = Path(directory)
        executables = {}
        copied = {}
        for index, source in enumerate(sources):
            target = temporary / f"unit{index:03d}.c"
            shutil.copyfile(source, target)
            copied[source.as_posix()] = target
            executable = temporary / f"unit{index:03d}.exe"
            process = subprocess.run([compiler, *flags, target.name, "-o", executable.name, "-lm"], cwd=temporary, env=environment, capture_output=True, timeout=45)
            diagnostic = process.stderr.decode("utf-8", errors="replace").replace(str(temporary), "<temporary>").replace(target.name, source.as_posix())
            builds.append({"source": source.as_posix(), "exit_code": process.returncode, "diagnostic": diagnostic})
            if process.returncode == 0:
                executables[source.as_posix()] = executable
            print(f"COMPILE {process.returncode}: {source.as_posix()}", flush=True)

        def run(name: str, suffix: str, input_text: str = "", files: dict | None = None, expected: list[str] | None = None, exit_code: int = 0) -> str:
            source = next(path for path in executables if path.endswith(suffix))
            working = temporary / f"case-{len(checks):02d}"
            working.mkdir()
            for filename, content in (files or {}).items():
                (working / filename).write_text(content, encoding="utf-8")
            process = subprocess.run([str(executables[source])], input=input_text.encode(), cwd=working, env=environment, capture_output=True, timeout=30)
            output = process.stdout.decode("utf-8", errors="replace").replace("\r\n", "\n")
            error = process.stderr.decode("utf-8", errors="replace").replace("\r\n", "\n")
            passed = process.returncode == exit_code and all(part in output + error for part in (expected or []))
            extra = {}
            if name == "huffman-roundtrip":
                original = (working / "input.txt").read_bytes()
                recovered = (working / "output.txt").read_bytes()
                passed = passed and original == recovered
                extra = {"original_bytes": len(original), "compressed_bytes": (working / "compressed.bin").stat().st_size, "byte_equal": original == recovered}
            if name == "segment-tree-query":
                pairs = re.findall(r"Segment Tree Max: (-?\d+), Brute Force Max: (-?\d+)", output)
                passed = passed and len(pairs) == 4 and all(a == b for a, b in pairs)
            checks.append({"name": name, "source": source, "passed": passed, "exit_code": process.returncode, "input": input_text, "stdout": output, "stderr": error, **extra})
            print(f"CHECK {passed}: {name}", flush=True)
            return output

        run("date-weekday", "/Programming_Assignment_I.c", "2019/9/20\n", expected=["Friday"])
        run("date-leap-day", "/Programming_Assignment_I.c", "2024/2/28 + 1\n", expected=["February 29, 2024"])
        run("date-backward", "/Programming_Assignment_I.c", "2024/3/1 + -1\n", expected=["February 29, 2024"])
        run("date-difference", "/Programming_Assignment_I.c", "2018/9/20 - 2019/9/20\n", expected=["365 days"])
        run("date-invalid", "/Programming_Assignment_I.c", "2023/2/29\n", expected=["Invalid input"], exit_code=1)
        run("maze-shortest-path", "/src/Programming_Assignment_III.c", "4 5\n2 x 0\n2 s 4 x 0\n3 x 0\n2 x 4 x 5 t 0\n", expected=["(2,2) -> (2,3) -> (1,3) -> (1,4) -> (1,5) -> (2,5) -> (3,5) -> (4,5)"])
        run("maze-unreachable", "/src/Programming_Assignment_III.c", "1 3\n1 s 2 x 3 t 0\n", expected=["No path found"])
        run("sparse-matrix", "/src/Programming_Assignment_IV.c", "2 2\n1 1 0\n1 1 0\n2 2\n2 2 0\n2 2 0\n", expected=["Result of A + B:\n1 2 \n1 2", "Result of A * B:\n0 2 \n0 2"])
        run("expression-values", "/src/Programming_Assignment_V.c", "1 + 2 * 3\n(1 + 2) * (3 - 4) / 5\na = 7\nb = 4 * a + 5\nb\n", expected=["Result: 7", "Result: -0.6", "Assigned a = 7", "Assigned b = 33", "Result: 33"])
        run("expression-undefined-current-behavior", "/src/Programming_Assignment_V.c", "z + 1\n", expected=["Result: 1"])
        run("expression-zero-division", "/src/Programming_Assignment_V.c", "10 / (5 - 5)\n", expected=["Division by zero"])
        run("segment-tree-query", "/src/Programming_Assignment_VI.c", "8\n4\n0 7\n2 5\n0 0\n7 7\n")
        run("huffman-roundtrip", "/src/7_14.c", expected=["files are identical"])
        graph = "5 6\n0 1 2\n0 2 5\n1 2 1\n1 3 3\n2 4 2\n3 4 1\n"
        run("kruskal-mst", "/src/8_5.c", files={"graph.txt": graph}, expected=["Total weight: 6"])
        output = run("dijkstra-shortest-path", "/src/9_4.c", input_text="\n", files={"graph.txt": graph}, expected=["Shortest Paths from Vertex 0:", "To Vertex 4: 0 -> 1 -> 2 -> 4 (Distance: 5)"])
        source_zero = output.split("Shortest Paths from Vertex 0:", 1)[1].split("Shortest path tree DOT", 1)[0]
        distances = [int(value) for value in re.findall(r"Distance: (\d+)", source_zero)]
        checks[-1]["passed"] = checks[-1]["passed"] and distances == [0, 2, 3, 5, 5]
        checks[-1]["source_zero_distances"] = distances
        run("gcd", "extras/0a-gcd.c", "48 18\n-48 18\n0 9\n", expected=["(48, 18) = 6", "(-48, 18) = 6", "(0, 9) = 9"])
        run("segment-tree-sum-extra", "extras/segment_tree.c", expected=["[1, 3]: 15", "after update: 16"])

        sorting = next(path for path in copied if path.endswith("/src/P2_2.c"))
        debug_executable = temporary / "sorting-debug.exe"
        process = subprocess.run([compiler, *flags, "-DDEBUG", copied[sorting].name, "-o", debug_executable.name, "-lm"], cwd=temporary, env=environment, capture_output=True, timeout=45)
        if process.returncode == 0:
            process = subprocess.run([str(debug_executable)], cwd=temporary, env=environment, capture_output=True, timeout=30)
            output = process.stdout.decode(errors="replace")
            correct = output.count("Array sorted correctly!")
            checks.append({"name": "sorting-five-algorithms", "source": sorting, "passed": process.returncode == 0 and correct == 5, "exit_code": process.returncode, "sorted_checks": correct, "stdout": output})
        else:
            checks.append({"name": "sorting-five-algorithms", "source": sorting, "passed": False, "diagnostic": process.stderr.decode(errors="replace")})

    result = {
        "checked_on": dt.datetime.now(dt.timezone(dt.timedelta(hours=8))).date().isoformat(),
        "compiler": version,
        "flags": flags,
        "builds": builds,
        "checks": checks,
        "summary": {"source_count": len(builds), "compiled": sum(item["exit_code"] == 0 for item in builds), "checks": len(checks), "passed": sum(item["passed"] for item in checks)},
    }
    (repository / "docs/verification.json").write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["summary"], indent=2))
    if not all(item["passed"] for item in checks):
        raise SystemExit(1)


if __name__ == "__main__":
    main()
