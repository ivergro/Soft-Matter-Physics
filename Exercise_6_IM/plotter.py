import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.patheffects as pe
import os
import numpy as np
import re
from scipy.optimize import curve_fit

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
    plt.xlabel('Timesteps ')
    # plt.ylabel('Energy')
    plt.title('T='+str(T))
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


def print_observables(filepath, filename_E, filename_M, T, L, save=False):
    match(L):
        case 25:
            t_eq = 2000
        case 50:
            t_eq = 4500
        case 100:
            t_eq = 2000
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
    return Cv, Nxt

def autocorrelation_time(filepath, filename_E, filename_M, L, t, save=False):
    match(L):
        case 25:
            t_eq = 2000
        case 50:
            t_eq = 4500
        case 100:
            t_eq = 2000
    energies =  np.array(load_vector(filepath + filename_E))[t_eq:]
    magnetizations = np.array(load_vector(filepath + filename_M))[t_eq:] #Cutting of data after equilibrium
    #Averages over each spin
    energies /= L**2
    magnetizations /= L**2

    def Co(t, O):
        Co_first = 0
        Co_second = 0
        Co_third = 0
        t_max = len(O)
        div_fac = 1/(t_max - t)
        for t_it in range(t_max-t):
            Co_first += O[t_it] * O[t_it + t] * div_fac
            Co_second += O[t_it] * div_fac
            Co_third += O[t_it + t] * div_fac

        return Co_first - Co_second*Co_third


    t_max = len(magnetizations)
    Cm_t = [Co(t_it, magnetizations) for t_it in range(t)]
    Ce_t = [Co(t_it, energies) for t_it in range(t)]
    ts = np.linspace(0,t, dtype=int, num=len(Cm_t))
    tau_mag = sum(Cm_t)
    tau_en = sum(Ce_t)
    print(f"Magnetic Co: {Co(t, magnetizations)}")
    print(f"Energy Co: {Co(t, energies)}")
    print(f"Tau for magnetization = {tau_mag}")
    print(f"len mag: {len(magnetizations)}, len cm_t: {len(Cm_t)}, len ts: {len(ts)}")
    print(f"Tau for energy = {tau_en}")

    #Plotting and fitting section
    f = lambda t,tau,a,b: a + b*np.e**(-t/tau) #Add a c?
    popt_mag, pcov_mag = curve_fit(f, ts, Cm_t )
    popt_en, pcov_en = curve_fit(f, ts, Ce_t)

    perr_mag = np.sqrt(np.diag(pcov_mag))
    perr_en = np.sqrt(np.diag(pcov_en))
    print(f"Tau for magnetization = {popt_mag[0]} ± {perr_mag[0]}")
    print(f"Tau for energy = {popt_en[0]} ± {perr_en[0]}")

    #---------Magnetization------------
    plt.clf()
    plt.plot(ts, Cm_t, label=r"$C_m(t)$")
    plt.plot(ts, f(ts, *popt_mag), label=r"$\tau$ fitted %3.3f ± %3.3f" % (popt_mag[0], perr_mag[0]))
    plt.xlabel("time (MC steps per spin)")
    plt.legend()
    if save:
        new_filepath = "./figures/L="+str(L)+"/" 
        new_filename_M = "T="+str(T)+"_M_tau_fitted_vec_"+str(t_eq)+"_to_"+str(t_eq+t_max-t)+".png"
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        plt.savefig(new_filepath+new_filename_M)
        print("Figure saved in local path: " + new_filepath)
    else:
        plt.show()
    #---------Energy-------------
    plt.clf()
    plt.plot(ts, Ce_t, label=r"$C_e(t)$")
    plt.plot(ts, f(ts, *popt_en), label=r"$\tau$ fitted = %3.3f ± %3.3f" % (popt_en[0], perr_en[0]))
    plt.xlabel("time (MC steps per spin)")
    plt.legend()
    if save:
        new_filepath = "./figures/L="+str(L)+"/" 
        new_filename_E = "T="+str(T)+"_E_tau_fitted_vec_"+str(t_eq)+"_to_"+str(t_eq+t_max-t)+".png"
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        plt.savefig(new_filepath+new_filename_E)
        print("Figure saved in local path: " + new_filepath)        
    else:
        plt.show()

def estimate_errors(filepath, filename_E, filename_M, L, save=False):
    match(L):
        case 25:
            t_eq = 2000
        case 50:
            t_eq = 4500
        case 100:
            t_eq = 2000
    energies =  np.array(load_vector(filepath + filename_E))[t_eq:]
    magnetizations = np.array(load_vector(filepath + filename_M))[t_eq:] #Cutting of data after equilibrium
    t_max = len(energies)
    #Averages over each spin
    energies /= L**2
    magnetizations /= L**2

    def error_bars(observable, t_max, tau, dt = 1):
        factor = (1+2*tau/dt)/(t_max - tau - 1)
        O_tmax = 1/(t_max - t_eq)*sum([observable[t] for t in range(t_eq, t_max)])
        print(f"O_tmax = {O_tmax}, with t_eq: {t_eq} and t_max: {t_max}")
        S_tmax_squared = (1+2*tau/dt)/(t_max - t_eq - 1) * sum([(observable[t] - O_tmax)**2 for t in range(t_eq, t_max)])
        return S_tmax_squared
    tau_en_25_tl = 38.408
    tau_mag_25_tl = 35.087
    print("Energy:")
    print(r"${S_{tmax}}^2 = %5.3f$ " % error_bars(energies, t_max, tau_en_25_tl))
    print("Magnetization:")
    print(r"${S_{tmax}}^2 = %5.3f$ " % error_bars(magnetizations, t_max, tau_mag_25_tl))



def cv_and_X_plot(filepath, sweeps, L, save = False):
    filename_E = filepath + "energies_Tnum="
    filename_M = filepath + "magnetizations_Tnum="
    rest_of_file = "_sweeps="+str(sweeps)+".txt"
    Energies = [np.array(load_vector(filename_E + str(Tnum) + rest_of_file)) for Tnum in range(1,21)]
    Magnetizations = [np.array(load_vector(filename_M + str(Tnum) + rest_of_file)) for Tnum in range(1,21)]
    T_actuals = np.linspace(0.1,5,20)
    T_c = 2/np.log(1+np.sqrt(2))
    Cvs = []
    Xs  = []
    kb = 1
    for i in range(len(Energies)):
        average_enery = sum(Energies[i])/len(Energies[i])
        average_enery_squared = sum((Energies[i])**2)/len(Energies[i])
        average_mag = sum(Magnetizations[i])/len(Magnetizations[i])
        average_mag_squared = sum((Magnetizations[i])**2)/len(Magnetizations[i])

        Cvs.append((average_enery_squared - average_enery**2)/(kb*T**2*L**2)) #Skal jeg ha L**2?
        Xs.append((average_mag_squared - average_mag**2)/(kb*T*len(Magnetizations[i])))
    
    plt.plot(T_actuals, Cvs, label="Cv")
    plt.plot(T_actuals, Xs, label="Susceptibility")
    plt.legend()
    plt.show()

def final_size_scaling(filepath, sweeps, Ls, save = False):
    magnetizations = [np.array(load_vector(filepath+"magnetizations_L="+str(L)+"_sweeps="+str(sweeps)+".txt")) for L in Ls]
    Xs = []
    M_v  = []
    kb = 1
    for i in range(len(Ls)):

        average_mag = sum(magnetizations[i])/len(magnetizations[i])
        average_mag_squared = sum((magnetizations[i])**2)/len(magnetizations[i])

        #Skal jeg ha L**2?
        Xs.append((average_mag_squared - average_mag**2)/(kb*T*len(magnetizations[i])*L**2))
        M_v.append(average_mag/(Ls[i]**2)) #assuming V=L**2
    M_v = np.array(M_v)
    plt.plot((Ls), (M_v))
    plt.plot(Ls, Xs)
    plt.show()

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
L = 25
sweeps = 10000
filepath = "./data/T={:.6f}/".format(T)
filename_t = "timesteps_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
filename_E = "energies_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
filename_M = "magnetizations_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
filename_spins = "saved_spin_values_L="+str(L)+"_sweeps="+str(sweeps)+".txt"

# plot_avgE_avgM_of_t(filepath, filename_E, filename_M, filename_t, True)
# show_2D_animation(filepath, filename_spins, sweeps, L, False)
# save_configurations(filepath, filename_spins, sweeps, L, False)


# plot_Cv(filepath_2, filename_Cv, filename_T)
# plot_susceptibility(filepath_2,filename_X,filename_T)
# print_observables(filepath, filename_E, filename_M)
# autocorrelation_time(filepath, filename_E, filename_M, L, 500)
# cv_and_X_plot("./data/tempsweeps/L="+str(L)+"/", sweeps,L, save = False)
# final_size_scaling("./data/T=2.269185/", 10000, np.array([25,50,100]))
estimate_errors(filepath, filename_E, filename_M, L)

#Saving many figs
# Ls = [100]
# Ts = [1.000000, T_c, 3.000000]
# sweeps = 10000
# for T in Ts:
#     filepath = "./data/T={:.6f}/".format(T)
#     for L in Ls:
#         filename_t = "timesteps_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
#         filename_E = "energies_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
#         filename_M = "magnetizations_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
#         autocorrelation_time(filepath, filename_E, filename_M, L, 500, True)
