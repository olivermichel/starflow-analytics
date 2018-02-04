set terminal pdf

data_file = 'output.dat';

stats 'output.dat' using 1 name 'tp_ps'

set output 'cdf.pdf';
plot data_file using 1:(1./tp_ps_records) smooth cumulative title 'CDF';

set output 'ts.pdf';
plot data_file using 1 with linespoints title 'records per second';
