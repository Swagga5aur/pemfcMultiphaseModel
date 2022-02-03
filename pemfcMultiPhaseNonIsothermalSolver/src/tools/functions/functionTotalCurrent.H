class functionTotalCurrent
{
    scalar Nernst_, Vcell_, R_, A_, i0_, B_, iL_;

    
public: 

    functionTotalCurrent(scalar Nernst, scalar Vcell, scalar R, scalar A, scalar i0, scalar B, scalar iL)
    :
    Nernst_(Nernst),
    Vcell_(Vcell),
    R_(R),
    A_(A),
    i0_(i0),
    B_(B),
    iL_(iL)
    {}

    scalar operator()(const scalar& i) const
    {
        return Nernst_-(i*R_)-(A_*Foam::log(i/i0_))-Vcell_-(B_*Foam::log(iL_/(iL_-i)));
    }
};


