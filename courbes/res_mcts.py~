import matplotlib.pyplot as plt
import numpy as np

prof = [10, 100, 1000, 5000]
rand = [0.231, 0.421, 0.745, 0.81]
mm1 = [0.03, 0.13, 0.271, 0.489]
mm3 = [0.00, 0.00, 0.11, 0.18]

plt.figure(figsize=(8, 6))
bar_width = 0.25
x_positions = np.arange(len(prof))

plt.bar(x_positions - bar_width/2, rand, bar_width, label='Fraction victoire RAND')
plt.bar(x_positions, mm1, bar_width, label='Fraction victoire MM1')
plt.bar(x_positions + bar_width/2, mm3, bar_width, label='Fraction victoire MM3')
plt.xticks(x_positions, prof)
plt.xlabel('profondeur')
plt.ylabel('Fraction de parties gagnées')
plt.ylim(0, 1.01)
plt.title('Résultat min-max')
plt.legend(loc='upper right')

plt.grid()
plt.show()
