def find_distance(time_list): 
    temps_freq = dict() 
    for temps in set(time_list):                   # pour chaque valeur unique de temps mesurée   
        temps_freq[temps] = time_list.count(temps) #  dict avec key temps et value frequence
    freq_max = max([v for k, v in temps_freq.items()])
    for k, v in temps_freq.items():
        if v == freq_max: 
            temps_commun = k
            break  
    # Cette méthode fait en sorte qu'on prend le temps plus petit si il y a plusieurs mesures avec le même count (distance plus petite plus safe) 
    distance_mm = int(((temps_commun - 74.691805951896)/5.43163473298))
    distance_cm = int(str(distance_mm)[:-1])
    #return distance_cm
    return len(time_list)