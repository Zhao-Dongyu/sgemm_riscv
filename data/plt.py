# pip install pandas matplotlib openpyxl

import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_excel('sgemm_riscv.xlsx', index_col=0)
# 绘制多行数据
plt.plot(df.loc['version 0'], label='version 0')
plt.plot(df.loc['version 1'], label='version 1')
plt.plot(df.loc['version 2'], label='version 2')
plt.plot(df.loc['version 3'], label='version 3')
plt.plot(df.loc['version 4'], label='version 4')
plt.plot(df.loc['version 5'], label='version 5')
plt.plot(df.loc['version 6'], label='version 6')
plt.plot(df.loc['version 7'], label='version 7')
plt.plot(df.loc['version 8'], label='version 8')
plt.plot(df.loc['version 9'], label='version 9')

plt.title("Sgemm on Nezha D1")
plt.xlabel("M=N=K")
plt.ylabel("GFLOPS")

plt.legend(loc='upper center', bbox_to_anchor=(0.5, -0.05), fancybox=True, shadow=True, ncol=5)

plt.tight_layout()

plt.show()

plt.savefig('result.png', dpi=300)