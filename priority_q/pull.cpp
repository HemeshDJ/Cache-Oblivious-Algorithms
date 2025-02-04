void priority_q::reg_pull(const ll lno, vector<T> &a)
{
    ll ind = level[lno].fir;
    ll min_sz = (level[lno].down_sz + 1)/2;

    desc_sort((level[lno].down_bf[ind].st));
    //take away least min_sz elements & put to a
    while(a.size() < min_sz)
    {
        a.push_back(level[lno].down_bf[ind].st.back());
        level[lno].down_bf[ind].st.pop_back();
        level[lno].cnt--;
    }
    //leave remaining in the buffer
    if(level[lno].down_bf[ind].st.size() == 0)
    {
        level[lno].fir = level[lno].down_bf[ind].next;
        level[lno].down_bf_cnt--;
    }
}

void priority_q::pull_from_above(const ll lno, vector<T> &a, vector<T> &b)
{
    for(T tt : level[lno].down_bf[ind].st)      //Take all elements from down_bf
            a.push_back(tt);
        
    level[lno].down_bf[ind].st.clear();
    level[lno].down_bf_cnt = 0;
    level[lno].cnt = 0;

    pull(lno+1, b);                     // Pull from next level

    ll u_sz = level[lno].up_bf.size();
    for(T tt : level[lno].up_bf)        //Take elements from up_bf
    {
        b.push_back(tt);
    }
    level[lno].up_bf.clear();
    asc_sort(b);                        //sort b and refill the first U elements into the up_bf;

    ll how_many_pulled = 0;
    for(ll i=0; a.size() < min_sz && i < b.size(); i++) //taking least [X]-Y keys and putting into a
    {
        a.push_back(b[i]);
        how_many_pulled++;
    }
    return how_many_pulled;
}

void refill_downbf(const ll lno, vector<ll> &b)
{
    if(b.size() - how_many_pulled == 0)     return;

    /* Refilling the first down_buffer of lno, size = rem & updating cnt of lno*/
    level[lno].cnt = b.size() - how_many_pulled;
    ll rem = level[lno].cnt % min_sz;               //No of elements to be put in down_bf[0]
    if(!rem)
        rem = min_sz;
    level[lno].down_bf_cnt++;
    ll ar_pos = how_many_pulled;
    ll curr_bf = 0;
    level[lno].fir = curr_bf;
    for(ll i=0; i<rem; i++)
    {
        level[lno].down_bf[curr_bf].st.push_back(b[ar_pos]);
        ar_pos++;
    }
    /* Refilling the remaining down_buffers of lno */
    ll how_many_bfs = (level[lno].cnt-rem)/min_sz;
    level[lno].down_bf_cnt += how_many_bfs;         //no of down_bfs = first + how_many_bfs
    for(ll i=1; i<=how_many_bfs; i++)
    {
        level[lno].down_bf[curr_bf].next = i;       //next of previous down_bf = i
        curr_bf = i;
        for(ll j=0; j<min_sz; j++)
        {
            level[lno].down_bf[curr_bf].st.push_back(b[ar_pos]);
            ar_pos++;
        }
        level[lno].down_bf[curr_bf].next = -1;      //next of the new down_bf = -1
    }
}

void priority_q::pull(const ll lno, vector<T> &a) //pull elements from level lno
{
    if(lno >= level.size())     return;

    ll ind = level[lno].fir;
    ll min_sz = (level[lno].down_sz + 1)/2;
    if(level[lno].down_bf_cnt > 0 && level[lno].down_bf[ind].st.size() >= min_sz)
    {
        reg_pull(lno,a);
    }
    else if(level[lno].down_bf_cnt > 1)
    {
        //pull first buffer fully
        while(level[lno].down_bf[ind].st.size() > 0)
        {
            a.push_back(level[lno].down_bf[ind].st.back());
            level[lno].down_bf[ind].st.pop_back();
            level[lno].cnt--;
        }
        /* Removing the first down_bf from the list */
        level[lno].fir = level[lno].down_bf[ind].next;
        level[lno].down_bf_cnt--;
        ind = level[lno].fir;                           //ind points to the current first down_bf
        reg_pull(lno,a);
    }
    else
    {
        vector<T> b;
        ll how_many_pulled = pull_from_above(lno,a,b);

        ll count = 0;   
        for(ll i=ll(b.size())-1; i>=how_many_pulled && count < u_sz; i--, count++)      //refilling upbuffer
        {
            level[lno].up_bf.push_back(b[i]);
            b.pop_back();
        }

        refill_downbf(lno,b);
    }
}
