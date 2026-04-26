import sys
from pathlib import Path


THIS_DIR = str(Path(__file__).resolve().parent)
sys.path = [p for p in sys.path if p not in ("", THIS_DIR)]

from sage.all import matrix_plot
from sage.crypto.sbox import SBox


LOG_FILENAME = "sboxes.log"
OUTPUT_DIRNAME = "pollock_plots"
DDT_CONTRAST_CLIP = 10
DDT_COLORMAP = "nipy_spectral"


def parse_sboxes(log_path):
	sboxes = []
	with open(log_path, "r", encoding="utf-8") as f:
		for line_no, raw in enumerate(f, start=1):
			line = raw.strip()
			if not line or line.startswith("#"):
				continue

			values = [int(x) for x in line.split()]
			size = len(values)
			if size == 0 or (size & (size - 1)) != 0:
				raise ValueError(
					f"Line {line_no}: expected a power-of-two number of entries, found {size}"
				)

			sboxes.append((line_no, values))

	if not sboxes:
		raise ValueError(f"No S-boxes found in {log_path}")

	expected_size = len(sboxes[0][1])
	for line_no, values in sboxes:
		if len(values) != expected_size:
			raise ValueError(
				f"Line {line_no}: expected {expected_size} entries, found {len(values)}"
			)

	return sboxes


def save_pollock_plots(sbox_values, index, ddt_dir):
	s = SBox(sbox_values)

	ddt = s.difference_distribution_table()
	ddt_vis = []
	for i in range(ddt.nrows()):
		row = []
		for j in range(ddt.ncols()):
			value = int(ddt[i, j])
			if i == 0 and j == 0:
				value = 0
			else:
				value = min(value, DDT_CONTRAST_CLIP)
			row.append(value)
		ddt_vis.append(row)

	ddt_plot = matrix_plot(
		ddt_vis,
		colorbar=True,
		axes=False,
		cmap=DDT_COLORMAP,
		vmin=0,
		vmax=DDT_CONTRAST_CLIP,
	)

	tag = f"sbox_{index:03d}"
	ddt_plot.save(str(ddt_dir / f"{tag}_ddt.png"), dpi=200)


def main():
	base_dir = Path(__file__).resolve().parent
	log_path = base_dir / LOG_FILENAME

	output_dir = base_dir / OUTPUT_DIRNAME
	ddt_dir = output_dir / "ddt"
	ddt_dir.mkdir(parents=True, exist_ok=True)

	sboxes = parse_sboxes(log_path)
	for idx, (line_no, values) in enumerate(sboxes, start=1):
		save_pollock_plots(values, idx, ddt_dir)
		print(f"Saved plots for S-box #{idx} (line {line_no})")

	print(f"Done. Pollock plots saved under: {output_dir}")


if __name__ == "__main__":
	main()
