import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

df = pd.read_csv("expense.csv")
category_total = df.groupby("category")["amount"].sum()


plt.figure(figsize=(12,5))


# for our bar chart
plt.subplot(1,2,1)
category_total.plot(kind="bar")
bars = plt.bar(category_total.index, category_total.values)
plt.title("Expense by Category")
plt.xlabel("Category")
plt.ylabel("Amount")
for bar in bars:
    height = bar.get_height()
    percentage = (height / category_total.sum()) * 100
    plt.text(
        bar.get_x() + bar.get_width()/2,
        height,
        f"{percentage:.1f}%",
        ha='center',
        va='bottom'
    )

# for our pie chart
plt.subplot(1,2,2)
plt.pie(category_total, labels=category_total.index, autopct="%1.1f%%")
plt.title("Expense Distribution by Category")


plt.ticklabel_format(style='plain', axis='y', useOffset=False)
plt.gca().yaxis.get_major_formatter().set_scientific(False)

plt.tight_layout()
plt.show()


print(df.dtypes)
print(category_total)