#pragma once
#include "PageManager.h"
#include "uvector.h"
#include "Syscall.h"
#include "Loader.h"


typedef enum{RANDOM, AGING, SECONDCHANCE}PRA_;

class PRA
{
    public:

    size_t max_Page_range = PageManager::instance()->getTotalNumPages();
    size_t min_Page_range = 0;
    size_t randomCount = 0, nfuCount = 0, secCount = 0, agingCount = 0, outCount = 0, inCount = 0;

    size_t pra_random_sample[2016] = {};
    size_t randomPRA_mean = 0;
    void switchCurrentPRA();
    void printPRAinfo();
    size_t agingPRA();
    size_t randomPRA();
    size_t secondChancePRA();
    size_t startPRA();

    //just for testing a single testcase
    size_t getMeanOfPRA();
    void printRandomSample();
    //double getSTDDeviationOfPRA();

    virtual ~PRA();
    static PRA *instance();
    Mutex count_lock;
    private:

    PRA();

    static PRA *instance_;
    size_t active_pra = PRA_::RANDOM;
};