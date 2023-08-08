import autorun_simul as ars 
import data_analysis as dt
def small_launch(path):

    #align
    ars.run_simul_nth(10,10,path,1,1000,"align:1","talig","ralig",990)
    dt.mean_results("ralig","res_align_conf")
    dt.clean_results("ralig")

    #prop + slow
    ars.run_simul_nth(10,10,path,1,1000,"mcrowd propu:1","tprop","rprop",990)
    dt.mean_results("rprop","res_prop_slow_conf")
    dt.clean_results("rprop")

    #rand
    ars.run_simul_nth(10,10,path,1,1000,"rand:1","trand","rran",990)
    dt.mean_results("rran","res_rand_conf")
    dt.clean_results("rran")

    # align + attra
    ars.run_simul_nth(10,10,path,1,1000,"align:9 attra:1","tbest","rbest",990)
    dt.mean_results("rbest","res_attalign_conf")
    dt.clean_results("rbest")

small_launch("conf_model.csv")
