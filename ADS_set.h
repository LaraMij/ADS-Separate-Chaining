#ifndef ADS_SET_H
#define ADS_SET_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>

template <typename Key, size_t N = 7>
class ADS_set {
public:
    class Iterator;
    using value_type = Key;
    using key_type = Key;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using const_iterator = Iterator;
    using iterator = const_iterator;
    //using key_compare = std::less<key_type>;                         // B+-Tree
    using key_equal = std::equal_to<key_type>;                       // Hashing
    using hasher = std::hash<key_type>;                              // Hashing


private:
    struct Element {
        key_type key;
        Element* next{ nullptr };
        Element* head{ nullptr };
        ~Element() {
            delete head;
            delete next;
        };
    };
    Element* table{ nullptr };
    size_type table_size{ 0 };
    size_type current_size{ 0 };
    float max_lf{ 0.6 };
    void add(const key_type& key);
   // bool locate(const key_type& key) const;
    Element* returnElement(const key_type& key) const;
    size_type h(const key_type& key) const { return hasher{}(key) % table_size; }
    //size_type g(const key_type&)const { return 1; } not needed for seperate chaining
    void reserve(size_type n);
    void rehash(size_type n);
public:                                                       
    ADS_set() : table{ new Element[N] }, table_size{ N }, current_size{ 0 } {}
    ADS_set(std::initializer_list<key_type> ilist) : ADS_set{} { insert(ilist); }                      
    template<typename InputIt> ADS_set(InputIt first, InputIt last) : ADS_set{} { insert(first, last); }     
    ADS_set(const ADS_set& other) : ADS_set(other.begin(), other.end()) {}
   
    ~ADS_set() { delete[] table; };

    ADS_set& operator=(const ADS_set& other);
    ADS_set& operator=(std::initializer_list<key_type> ilist);
    size_type size() const { return current_size; }                                     
    bool empty() const { return current_size == 0; }                                              
    void insert(std::initializer_list<key_type> ilist) { insert(ilist.begin(), ilist.end()); }
    std::pair<iterator, bool> insert(const key_type& key);
    template<typename InputIt> void insert(InputIt first, InputIt last); 
    void clear();
    size_type erase(const key_type& key);
    size_type count(const key_type& key) const {
        if (returnElement(key) != nullptr) { return 1; }
        else return 0;
    }                          
    iterator find(const key_type& key) const{
        if (auto e{ returnElement(key) }) {
            auto idx = h(key);
            return iterator{ table, e, table_size, idx };
        }
        return end();
    }
    void swap(ADS_set& other);

    const_iterator begin() const {
        for (size_t i{ 0 }; i < table_size; i++) {
            if (table[i].head != nullptr) {
                return const_iterator(table, table[i].head, table_size, i);
            }
        } return end();
    }

    const_iterator end() const {
        return const_iterator(table, nullptr, table_size, table_size);
    }

    void dump(std::ostream& o = std::cerr) const;

    friend bool operator==(const ADS_set& lhs, const ADS_set& rhs) {
        if (lhs.current_size != rhs.current_size) return false;
        for (const auto& k : lhs) if (!rhs.count(k)) return false;

        return true;
    }
    friend bool operator!=(const ADS_set& lhs, const ADS_set& rhs) { return !(lhs == rhs); }
};


template <typename Key, size_t N>
void ADS_set<Key, N>::add(const key_type& key) {
    size_type idx{ h(key) };
    Element* newe = new Element;
    newe->next = nullptr;
    newe->key = key;
    Element* helper = table[idx].head;
    if (table[idx].head == nullptr) {
        table[idx].head = newe;
    }
    else {
        newe->next = helper->next;
        helper->next = newe;
    }
    ++current_size;
}

template <typename Key, size_t N>
typename ADS_set<Key, N>::Element* ADS_set<Key, N>::returnElement(const key_type& key) const {
    size_type newkey = h(key);
    Element* helper = table[newkey].head;
    while (helper != nullptr && !(key_equal{}(helper->key, key))) {
        helper = helper->next;
    }
    if (helper == nullptr) return nullptr;
    else return helper; 
}
/*
template <typename Key, size_t N>
bool ADS_set<Key, N>::locate(const key_type& key) const {
    size_type newkey = h(key);
    Element* helper = table[newkey].head;
    while (helper != nullptr && !(key_equal{}(helper->key, key))) {
        helper = helper->next;
    }
    if (helper == nullptr) return false;
    return true;
}
*/
template <typename Key, size_t N>
template<typename InputIt> void ADS_set<Key, N>::insert(InputIt first, InputIt last) {
    for (auto it{ first }; it != last; ++it) {
        if (returnElement(*it)==nullptr) {
            reserve(current_size + 1);
            add(*it);
        }
    }
}

template <typename Key, size_t N>
void ADS_set<Key, N>::reserve(size_type n) {
    if (table_size * max_lf >= n) return;
    size_type new_table_size{ table_size };
    do { ++(new_table_size *= 2); } while (new_table_size * max_lf < n);
    rehash(new_table_size);
}

template <typename Key, size_t N>
void ADS_set<Key, N>::rehash(size_type n) {
    size_type new_table_size{ std::max(N,std::max(n,size_type(current_size / max_lf))) };
    Element* new_table{ new Element[new_table_size + 1] };
    Element* old_table{ table };
    size_type old_table_size{ table_size };

    current_size = 0;
    table = new_table;
    table_size = new_table_size;

    for (size_type idx{ 0 }; idx < old_table_size; ++idx) {
        Element* helper = old_table[idx].head;
        while (helper != nullptr) {
            add(helper->key);
            helper = helper->next;
        }
    }
    delete[] old_table;
}



template <typename Key, size_t N>
ADS_set<Key, N>& ADS_set<Key, N>::operator=(const ADS_set& other) {
    ADS_set tmp{ other };
    swap(tmp); 
    return *this;
}

template <typename Key, size_t N>
ADS_set<Key, N>& ADS_set<Key, N>::operator=(std::initializer_list<key_type> ilist) {
    ADS_set tmp{ ilist };
    swap(tmp);
    return *this;
}
template <typename Key, size_t N>
std::pair<typename ADS_set<Key, N>::iterator, bool> ADS_set<Key, N>::insert(const key_type& key) {
    size_type idx = h(key);
    if (auto e{ returnElement(key) }) {
        return { iterator{table,e,table_size,idx}, false };
    }else{
    reserve(current_size + 1);
    add(key);
    idx = h(key);
    Element *helper = returnElement(key);
    return { iterator{table,helper,table_size,idx}, true };
    }
}

template<typename Key, size_t N>
void ADS_set<Key, N>::clear() {
    ADS_set tmp;
    swap(tmp);
}

template <typename Key, size_t N>
typename ADS_set<Key, N>::size_type ADS_set<Key, N>::erase(const key_type &key) {
    size_type ck = h(key);
    Element* first = table[ck].head;
    Element* current = table[ck].head;
    Element* previous = nullptr;

    if (returnElement(key)!=nullptr) {
        if (key_equal{}(first->key, key)) {
            table[ck].head = first->next;
            first->next = nullptr;
            delete first;

            --current_size;
            return 1;
        }
        while (current != nullptr) {
            if (key_equal{}(current->key, key)) {
                if (previous!=nullptr) {
                    previous->next = current->next;
                    current->next = nullptr;
                    delete current;

                    --current_size;
                    return 1;
                }
            }
            previous = current;
            current = current->next;
        }
    } return 0;
}


/*
template <typename Key, size_t N>
typename ADS_set<Key, N>::iterator ADS_set<Key, N>::find(const key_type& key) const {
    if (auto e{ returnElement(key) }) {
        auto idx = h(key);
        return iterator{ table, e, table_size, idx };
    }
    return end();
}*/

template <typename Key, size_t N>
void ADS_set<Key, N>::swap(ADS_set& other) {
    using std::swap;
    swap(table, other.table);
    swap(table_size, other.table_size);
    swap(current_size, other.current_size);
    swap(max_lf, other.max_lf);
}

template <typename Key, size_t N>
void ADS_set<Key, N>::dump(std::ostream& o) const {
    o << "table_size = " << table_size << ", current_size = " << current_size << "\n";

    for (size_type i = 0; i < table_size; i++) {
        o << i << ": ";
        Element* helper = table[i].head;
        if (helper == nullptr) {
            o << "N/A";
        }
        else {
            while (helper != nullptr) {
                o << helper->key << " -> ";
                helper = helper->next;
            }
        }
        o << "\n";
    }
}




template <typename Key, size_t N>
class ADS_set<Key, N>::Iterator {
    Element* table;
    Element* currentElement;
    size_type table_size;
    size_type idx;

    bool hasReachedTableEnd() {
        Element* helper = table[table_size].head;
        if (helper == currentElement) return true;
        else return false;
    }

    bool hasReachedLastElementInLinkedList() {
        if (currentElement->next == nullptr) return true;
        else return false;
    }

public:
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using reference = const value_type&;
    using pointer = const value_type*;
    using iterator_category = std::forward_iterator_tag;

    explicit Iterator(Element* table = nullptr, Element* currentElement = nullptr, size_type table_size = 0, size_type idx = 0) : table{ table }, currentElement{ currentElement }, table_size{ table_size }, idx{ idx } {}

    reference operator*() const {
        return currentElement->key;
    }

    pointer operator->() const {
        return &currentElement->key;
    } //returns adress of the key

    Iterator& operator++() {
        if (hasReachedLastElementInLinkedList()) {
            
            do {
                ++idx;
                currentElement = table[idx].head;

            } while (currentElement == nullptr && idx < table_size);
            
            if (hasReachedTableEnd()) {
                currentElement = nullptr;
                return *this;
            }

        } else {
            currentElement = currentElement->next;
        }
        
        return *this;
    }

    Iterator operator++(int) {
        auto rc{ *this };
        ++* this;
        return rc;
    }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
        return lhs.currentElement == rhs.currentElement;
    }

    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
        return !(lhs == rhs);
    }

};

template <typename Key, size_t N>
void swap(ADS_set<Key, N>& lhs, ADS_set<Key, N>& rhs) { lhs.swap(rhs); }

#endif // ADS_SET_H