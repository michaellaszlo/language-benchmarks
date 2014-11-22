template<typename Iterator>
void displayVector(Iterator it, Iterator end) {
    cout << *it;
    for (++it; it != end; ++it) {
        cout << " " << *it;
    }
    cout << '\n';
}
