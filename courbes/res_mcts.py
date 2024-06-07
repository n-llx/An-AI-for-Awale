import matplotlib.pyplot as plt
import numpy as np

prof = [10, 100, 1000, 5000]
rand = [0.81, 1, 1, 1]
mm3 = [0.18, 0.47, 0.79, 0.85]
mm5 = [0.05, 0.28, 0.58, 0.70]

plt.figure(figsize=(8, 6))
bar_width = 0.25
x_positions = np.arange(len(prof))

plt.bar(x_positions - bar_width/2, rand, bar_width, label='Fraction victoire RAND')
plt.bar(x_positions, mm3, bar_width, label='Fraction victoire MM3')
plt.bar(x_positions + bar_width/2, mm5, bar_width, label='Fraction victoire MM5')
plt.xticks(x_positions, prof)
plt.xlabel('profondeur')
plt.ylabel('Fraction de parties gagnées')
plt.ylim(0, 1.01)
plt.title('Résultat recherche arborescente de Monte-Carlo')
plt.legend(loc='upper right')

plt.grid()
plt.show()
