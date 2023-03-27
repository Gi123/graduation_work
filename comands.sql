CREATE TABLE default.valpot (
    rid UInt64,
    pid UInt64,
    mtime DateTime DEFAULT now(),
    comp String,
    val Float64
) ENGINE = MergeTree() ORDER BY (rid, pid, mtime)

INSERT INTO default.valpot (rid, pid, comp, val) VALUES (2, 2, 'Noise', 0.5), (2, 2, 'Voltage', 170.3), (2, 1, 'UNorm', 4.100)