/**
 * return array with only specified range of elements from another
 * array
 * 
 * @param vec array to take elements from
 * @param start index to start taking elements from
 * @param end index to stop taking elements
 * @returns vector of specified elements
 */
template<class T>
std::vector<T> stringutils::subarr(std::vector<T> vec, int start, int end) {
    std::vector<T> mod;
    for(int i = start; i < end; i++) {
        mod.push_back(vec.at(i));
    }
    return mod;
}