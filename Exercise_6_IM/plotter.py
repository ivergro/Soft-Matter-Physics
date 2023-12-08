import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.patheffects as pe
import os
import numpy as np
import re

def plot_avgE_avgM_of_t(filepath, filename_E, filename_M, filename_t, save=False):
    # Create a plot of energy vs. timesteps
    timesteps = np.array(load_vector(filepath + filename_t))
    energies =  np.array(load_vector(filepath + filename_E))
    magnetizations = np.array(load_vector(filepath + filename_M))
    int_values = [match.group() for match in re.finditer(r'\d+', filepath + filename_E)] #Saving L value in filename to take averages
    print(int_values)
    N = int(int_values[2])**2
    T = float(int_values[0] + "." + int_values[1])
    print(N, T)
    avg_energies = energies/N
    avg_magnetization = magnetizations/N
    plt.plot(timesteps, avg_energies, linestyle='-', color='g', label=r'$\overline{E}$')
    plt.plot(timesteps, avg_magnetization, linestyle='-', color='r', label=r'$\overline{M}$')

    # Add labels and a legend
    plt.xlabel('Timesteps')
    # plt.ylabel('Energy')
    plt.title(r'$\overline{E}$ across time, with T='+str(T))
    plt.legend()

    # Show the plot or save it to a file
    if save:
        new_filepath = "./figures/L="+str(L)+"/" 
        new_filename = "T="+str(T)+"_E_and_M_sweeps="+str(sweeps)+".png"
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        plt.savefig(new_filepath+new_filename)
        print("Figure saved in local path: " + new_filepath)
    else:
        plt.show()

def plot_Cv(filepath, filename_Cv, filename_T, save=False):
    # Create a plot of specific_heat vs. temp
    Temps = np.array(load_vector(filepath + filename_T))
    specific_heat =  np.array(load_vector(filepath + filename_Cv))
    int_values = [match.group() for match in re.finditer(r'\d+', filepath)] #Saving L value in filename to take averages

    plt.plot(Temps, specific_heat, linestyle='-', color='g', label=r'C_V(T)')

    # Add labels and a legend
    plt.xlabel('Temperature')
    # plt.ylabel('Energy')
    plt.title('Specific heat for L='+int_values[0])
    plt.legend()

    # Show the plot or save it to a file
    # plt.show()  # Use this to display the plot interactively
    if save:
        new_filepath = "./figures/ex_6/" + filename_Cv[:-4]
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        plt.savefig(new_filepath)
        if not os.path.exists(filepath + filename_Cv):
            print(f"Could not create the file '{filepath + filename_Cv}'")
        else:
            print("Figure saved in local path: " + new_filepath)
    else:
        plt.show()

def plot_susceptibility(filepath, filename_X, filename_T, save=False):
    # Create a plot of specific_heat vs. temp
    Temps = np.array(load_vector(filepath + filename_T))
    specific_heat =  np.array(load_vector(filepath + filename_X))
    int_values = [match.group() for match in re.finditer(r'\d+', filepath)] #Saving L value in filename to take averages

    plt.plot(Temps, specific_heat, linestyle='-', color='g', label=r'X(T)')

    # Add labels and a legend
    plt.xlabel('Temperature')
    # plt.ylabel('Energy')
    plt.title('Susceptibility for L='+int_values[0])
    plt.legend()

    # Show the plot or save it to a file
    # plt.show()  # Use this to display the plot interactively
    if save:
        new_filepath = "./figures/ex_6/" + filename_X[:-4]
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        plt.savefig(new_filepath)
        if not os.path.exists(filepath + filename_X):
            print(f"Could not create the file '{filepath + filename_X}'")
        else:
            print("Figure saved in local path: " + new_filepath)
    else:
        plt.show()

def print_observables(filepath, filename_E, filename_M, save=False):
    t_eq = 4500
    energies =  np.array(load_vector(filepath + filename_E))[t_eq:]
    magnetizations = np.array(load_vector(filepath + filename_M))[t_eq:] #Cutting of data before equilibrium

    kb = 1
    int_values = [match.group() for match in re.finditer(r'\d+', filepath + filename_E)] #Saving L value in filename to take averages
    T = float(int_values[0] + "." + int_values[1])

    average_enery = sum(energies)/len(energies)
    average_enery_squared = sum(energies**2)/len(energies)
    average_mag = sum(magnetizations)/len(magnetizations)
    average_mag_squared = sum(magnetizations**2)/len(magnetizations)

    Cv = (average_enery_squared - average_enery**2)/(kb*T**2*L**2) #Skal jeg ha L**2?
    Nxt = (average_mag_squared - average_mag**2)/(kb*T*len(magnetizations))
    print("Results for L=", L, " and T=", T, ":")
    print("-----------------------------------")
    print("Specific heat Cv = ", Cv)
    print("Magneticusceptibily: ", Nxt)
    print("-----------------------------------")

def autocorrelation_time(filepath, filename_E, filename_M, t, save=False):
    t_eq = 4000
    energies =  np.array(load_vector(filepath + filename_E))[t_eq:]
    magnetizations = np.array(load_vector(filepath + filename_M))[t_eq:] #Cutting of data before equilibrium

    kb = 1
    def Co(t, O):
        Co_first = 0
        Co_second = 0
        Co_third = 0
        t_max = len(O)
        div_fac = 1/(t_max - t)
        if t != 0:
            for t_it in range(t_max-t):
                Co_first += O[t_it] * O[t_it + 1] * div_fac
                Co_second += O[t_it] * div_fac
                Co_third += O[t_it + 1] * div_fac
        else:
            for t_it in range(t_max-t):
                Co_first += O[t_it] * O[t_it] * div_fac
                Co_second += O[t_it] * div_fac
                Co_third += O[t_it] * div_fac
        return Co_first - Co_second*Co_third

    tau_mag = sum([Co(t_it, magnetizations/len(magnetizations)) for t_it in range(t)])
    tau_en = sum([Co(t_it, energies/len(energies)) for t_it in range(t)])
    print(f"Magnetic Co: {Co(t, magnetizations/len(magnetizations))}")
    print(f"Energy Co: {Co(t, energies/(len(energies)))}")
    print(f"Tau for magnetization = {tau_mag}")
    print(f"Tau for energy = {tau_en}")


def save_configurations(filepath, filename, sweeps, L, save=False):
    data = load_3D_vector(filepath + filename)
    dt = int(sweeps/len(data))
    T = float([match.group() for match in re.finditer(r'\d+', filepath + filename_E)][0]) #super stupid but works
    save_frames = [0,len(data)//2,len(data) - 1] #first, middle, last

    fig, ax = plt.subplots()

    for el in data[save_frames[1]]:
        print(len(el))
        if len(el) > 25:
            print(el)
    if save:
        pass
    else:
        for pos in save_frames:
            lattice_array = np.array(data[pos], dtype=float)   

            print("pos: ", pos, "\tlen=", len(data[pos]), "\n", data[pos])
            ax.imshow(lattice_array, cmap='bwr', interpolation='none', label='Spin values')
            ax.set_title('Spin values for t='+str(dt*(pos+1)))
            ax.text(0.02, 0.95, '', transform=ax.transAxes, path_effects=[pe.withStroke(linewidth=3, foreground='w')])
            plt.show()

def show_2D_animation(filepath, filename, sweeps, L, save=False):
    data = load_3D_vector(filepath + filename)

    # shapes = [np.array(i).shape for i in data]
    # if len(set(shapes)) > 1:
    #     print("Data contains sub-arrays with different shapes")
    # else:
    #     print("Data contains same shapes: ", shapes)

    dt = int(sweeps/len(data))
    T = float([match.group() for match in re.finditer(r'\d+', filepath + filename_E)][0]) #super stupid but works
    
    fig, ax = plt.subplots()

    img = ax.imshow(data[0], cmap='bwr', interpolation='none', label='Spin values')
    ax.set_title('Spin values across time')
    time_text = ax.text(0.02, 0.95, '', transform=ax.transAxes, path_effects=[pe.withStroke(linewidth=3, foreground='w')])

    def update_2D_plot(frame, data, img):
        img.set_array(data[frame])
        time_text.set_text('Sweeps: {}'.format(frame * dt))
        return img, time_text
    
    ani = animation.FuncAnimation(fig, update_2D_plot, frames=len(data), fargs=(data, img), interval=500, blit=True)
    if save:
        
        ani.save("./figures/animations/L="+str(L)+"/spins_T="+str(T)+"_sweeps="+str(sweeps)+".gif", writer='Pillow', fps=60)
    else:
        plt.show()



#----------File loaders--------------
def load_vector(filename):
    if not os.path.exists(filename):
        print(f"ERROR: File '{filename}' not found")
    else:
        with open(filename, "r") as file:
            # Read the entire file as a string
            file_contents = file.read()

        # Split and save values to list
        lines = file_contents.split("\n")
        list = [float(item) for item in lines]
        return list

# def load_3D_vector(filename):
#     if not os.path.exists(filename):
#         print(f"ERROR: File '{filename}' not found")
#     else:
#         with open(filename, 'r') as f:
#             array_3d = []
#             for line in f:
#                 matrix = [list(map(int, row.split())) for row in line.strip().split(',')]
#                 array_3d.append(matrix)
#         print(np.array(array_3d).shape)
#         return np.array(array_3d, dtype=object)
# def load_3D_vector(filename):
#     with open(filename, 'r') as f:
#         data = f.read().split('\n')  # split matrices
#         data = [matrix.split(',') for matrix in data]  # split rows
#         data = [[row.split() for row in matrix] for matrix in data]  # split elements
#         data = [[[float(element) for element in row] for row in matrix] for matrix in data]  # convert to float
#     return data
def load_3D_vector(filename):
    if not os.path.exists(filename):
        print(f"ERROR: File '{filename}' not found")
    else:
        with open(filename, 'r') as f:
            configurations = f.read().split("\n") #list over all final spin matrices len=sweeps/100
            
            lattices = []
            for matrices in configurations:
                temp_matrice = matrices.split(",")
                for i in range(len(temp_matrice)):
                    try:
                        temp_matrice[i] =  list(map(float,temp_matrice[i].split()))
                    except(ValueError): #Nice to have since some blanksoaces fucks up the code
                        print(temp_matrice[i])
                        print(temp_matrice[i].split(" "))
                        print(i)
                lattices.append(temp_matrice)
    return lattices

#-------------------------------------


T_c = 2.269185
T = 3.000000
L = 50
sweeps = 10000
filepath = "./data/T={:.6f}/".format(T_c)
filename_t = "timesteps_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
filename_E = "energies_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
filename_M = "magnetizations_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
filename_spins = "saved_spin_values_L="+str(L)+"_sweeps="+str(sweeps)+".txt"

# plot_avgE_avgM_of_t(filepath, filename_E, filename_M, filename_t, True)
# show_2D_animation(filepath, filename_spins, sweeps, L, False)
# save_configurations(filepath, filename_spins, sweeps, L, False)

filepath_2 = "./data/L="+str(L)+"/"
filename_Cv = "specific_heat_sweeps="+str(sweeps)+".txt"
filename_X = "susceptibility_sweeps="+str(sweeps)+".txt"
filename_T = "Temps"


# plot_Cv(filepath_2, filename_Cv, filename_T)
# plot_susceptibility(filepath_2,filename_X,filename_T)
# print_observables(filepath, filename_E, filename_M)
autocorrelation_time(filepath, filename_E, filename_M, 2000)