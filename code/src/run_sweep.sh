for p in 1 2 8 16; do
  # patch the exponent
  sed -i '' "s/cross_talk_cos\[i\]\[j\] = .*;/cross_talk_cos[i][j] = std::pow(s, $p);  \/\/ |sin|^$p/" Problem.cpp
  g++ -std=c++17 -O2 -Wall -o main Problem.cpp Arrangement.cpp GreedySolver.cpp BeamSearchSolver.cpp main.cpp
  ./main 2>/dev/null
  cp results.csv "results_p${p}.csv"
  echo "=== p=$p ==="
  awk -F',' 'NR>1 && $2==1 {same+=($4==$5); total++} END {printf "B=1==greedy: %d/%d\n", same, total}' results.csv
  awk -F',' 'NR>1 && $5<$4 {bad++} NR>1 {total++} END {printf "beam<greedy: %d/%d (%.1f%%)\n", bad+0, total, (bad+0)/total*100}' results.csv
  awk -F',' 'NR>1 {diff[int($2)]+=($5-$4)/$4*100; cnt[int($2)]++} END {for(b=1;b<=10;b++) printf "  B=%2d: %+.2f%%\n", b, diff[b]/cnt[b]}' results.csv
done
