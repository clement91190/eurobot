class Period
{
    public:
        Period(long period_);
        bool is_over();
        void reset();
    private:
        long period;
        long time_last_reset;

};


Period::Period(long period_):period(period_)
{
    reset();
}


void Period::reset()
{
    time_last_reset = millis();
}

bool Period::is_over(){
    long t = millis();
    if (t - time_last_reset > period){
    return true;
    }
    else
    {
    return false;
    }

}
