import matplotlib.pyplot as plt
import numpy as np

prof = [1, 2, 3, 4, 5]
rand = [0.892, 0.974, 0.987, 1, 1]
mm3 = [0.342, 0.401, 0.498, 0.624, 0.842]

plt.figure(figsize=(8, 6))
bar_width = 0.4
x_positions = np.arange(len(prof))

plt.bar(x_positions - bar_width/2, rand, bar_width, label='Fraction victoire RAND')
plt.bar(x_positions + bar_width/2, mm3, bar_width, label='Fraction victoire MM3')
plt.xticks(x_positions, prof)
plt.xlabel('profondeur')
plt.ylabel('Fraction de parties gagnées')
plt.ylim(0.25, 1.01)
plt.title('Résultat min-max')
plt.legend(loc='lower right')

plt.grid()
plt.show()
