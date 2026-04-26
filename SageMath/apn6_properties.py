from sage.crypto.sbox import SBox

LOG_PATH = "apn6.log"
OUTPUT_PATH = "apn6_properties.txt"
NUM_SBOXES = 100
EXPECTED_SIZE = 64


def parse_sboxes(path, limit):
    sboxes = []
    with open(path, "r", encoding="utf-8") as f:
        for line_no, raw in enumerate(f, start=1):
            if len(sboxes) >= limit:
                break
            line = raw.strip()
            if not line:
                continue
            values = [int(x) for x in line.split()]
            if len(values) != EXPECTED_SIZE:
                raise ValueError(
                    f"Line {line_no}: expected {EXPECTED_SIZE} entries, found {len(values)}"
                )
            sboxes.append((line_no, values))
    return sboxes


def compute_properties(values):
    s = SBox(values)
    return {
        "Differential Uniformity": s.differential_uniformity(),
        "Algebraic Degree": s.max_degree(),
        "Non Linearity": s.nonlinearity(),
        "Linearity": s.linearity(),
        "Boomerang Uniformity": s.boomerang_uniformity(),
    }


def main():
    sboxes = parse_sboxes(LOG_PATH, NUM_SBOXES)
    output_lines = []

    def emit(line):
        output_lines.append(line)

    if len(sboxes) < NUM_SBOXES:
        emit(f"Warning: found only {len(sboxes)} S-boxes in {LOG_PATH}")

    for idx, (line_no, values) in enumerate(sboxes, start=1):
        props = compute_properties(values)
        emit(f"S-box #{idx} (apn6.log line {line_no})")
        for key, val in props.items():
            emit(f"{key}: {val}")
        emit("-" * 40)

    with open(OUTPUT_PATH, "w", encoding="utf-8") as f:
        f.write("\n".join(output_lines) + "\n")


main()
