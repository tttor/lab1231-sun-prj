#!/usr/bin/python
import numpy as np 

# method = 'GBR'
# mse_list_filepath = '/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-result/philippunarymrf-voc2010-GradientBoostingRegressor-20150501.201503.s/mse.all'
# r2_list_filepath = '/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-result/philippunarymrf-voc2010-GradientBoostingRegressor-20150501.201503.s/r2.all'
# n_optimistic_list_filepath = '/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-result/philippunarymrf-voc2010-GradientBoostingRegressor-20150501.201503.s/n_optimistic.all'
# n_pesimistic_list_filepath = '/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-result/philippunarymrf-voc2010-GradientBoostingRegressor-20150501.201503.s/n_pesimistic.all'

# 
method = 'nu-SVR'
mse_list_filepath = '/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-result/philippunarymrf-voc2010-NuSVR-20150501.200124.s/mse.all'
r2_list_filepath = '/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-result/philippunarymrf-voc2010-NuSVR-20150501.200124.s/r2.all'
n_optimistic_list_filepath = '/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-result/philippunarymrf-voc2010-NuSVR-20150501.200124.s/n_optimistic.all'
n_pesimistic_list_filepath = '/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-result/philippunarymrf-voc2010-NuSVR-20150501.200124.s/n_pesimistic.all'

def main():
    print 'method=', method

    print 'n_pesimistic vs n_optimistic'
    n_pesimistic_list = np.genfromtxt(n_pesimistic_list_filepath, delimiter=',')
    n_optimistic_list = np.genfromtxt(n_optimistic_list_filepath, delimiter=',')

    n_pesimistic_mean = np.mean(n_pesimistic_list)
    n_optimistic_mean = np.mean(n_optimistic_list)

    ratio_pesimistic = float(n_pesimistic_mean)/(n_pesimistic_mean+n_optimistic_mean)
    ratio_optimistic = float(n_optimistic_mean)/(n_pesimistic_mean+n_optimistic_mean)

    print 'ratio_pesimistic=', ratio_pesimistic
    print 'ratio_optimistic=', ratio_optimistic

    print '---mse---'
    mse_list = np.genfromtxt(mse_list_filepath, delimiter=',')
    print 'mean=', np.mean(mse_list)
    print 'std=', np.std(mse_list)
    print 'max=', np.max(mse_list)
    print 'min=', np.min(mse_list)

    print '---r2---'
    r2_list = np.genfromtxt(r2_list_filepath, delimiter=',')
    print 'mean=', np.mean(r2_list)
    print 'std=', np.std(r2_list)
    print 'max=', np.max(r2_list)
    print 'min=', np.min(r2_list)

if __name__ == '__main__':
    main()