#include "hash_dictionary.hpp"

// prime test for use in add
#include <cmath>
bool isprime(std::size_t index) {
    
    if ((index == 2) || (index == 3)) {
        return true;
    }
    if ((index % 2) == 0 || (index % 3) == 0) {
        return false;
    }
    std::size_t i = 5;
    while (i * i <= index) {
        if ((index % i) == 0 || (index % (i + 2) == 0)) {
            return false;
        }
        i += 6;
    }
    return true;
}


template <typename KeyType, typename ValueType, typename HashType>
HashDictionary<KeyType, ValueType, HashType>::HashDictionary(
                                                             HashType hash, std::size_t initial_capacity, float load_factor) {
    m_hash = hash;
    m_load_factor = load_factor;
    m_capacity = initial_capacity;
    m_size = 0;
    m_data = new KeyValueType[m_capacity];
}

template <typename KeyType, typename ValueType, typename HashType>
HashDictionary<KeyType, ValueType, HashType>::HashDictionary(
                                                             const HashDictionary &rhs) {
    
    m_load_factor = rhs.load_factor;
    m_capacity = rhs.m_capacity;
    m_size = rhs.m_size;
    m_data = new KeyValueType[m_capacity];
    for (std::size_t i = 0; i < m_size; ++i) {
        m_data[i] = rhs.m_data[i];
    }
}

template <typename KeyType, typename ValueType, typename HashType>
HashDictionary<KeyType, ValueType, HashType>::HashDictionary(
                                                             HashDictionary &&rhs) {
    std::swap(m_load_factor, rhs.m_load_factor);
    std::swap(m_capacity, rhs.m_capacity);
    std::swap(m_size, rhs.m_size);
    std::swap(m_data, rhs.m_data);
}

template <typename KeyType, typename ValueType, typename HashType>
HashDictionary<KeyType, ValueType, HashType>::~HashDictionary() {
    delete[] m_data;
}

template <typename KeyType, typename ValueType, typename HashType>
HashDictionary<KeyType, ValueType, HashType> &
HashDictionary<KeyType, ValueType, HashType>::operator=(
                                                        const HashDictionary &rhs) {
    
    if (this == &rhs)
        return *this;
    
    if (m_capacity != rhs.m_capacity) {
        delete[] m_data;
        m_capacity = rhs.m_capacity;
        m_data = new KeyValueType[m_capacity];
    }
    m_size = rhs.m_size;
    for (std::size_t i = 0; i < m_size; ++i) {
        m_data[i] = rhs.m_data[i];
    }
    
    return *this;
}

template <typename KeyType, typename ValueType, typename HashType>
HashDictionary<KeyType, ValueType, HashType> &
HashDictionary<KeyType, ValueType, HashType>::operator=(HashDictionary &&rhs) {
    
    // tear down current instance, make empty
    delete[] m_data;
    m_data = nullptr;
    m_capacity = 0;
    m_size = 0;
    
    // steal contents of rhs
    std::swap(m_load_factor, rhs.m_load_factor);
    std::swap(m_capacity, rhs.m_capacity);
    std::swap(m_size, rhs.m_size);
    std::swap(m_data, rhs.m_data);
    
    return *this;
}

template <typename KeyType, typename ValueType, typename HashType>
bool HashDictionary<KeyType, ValueType, HashType>::isEmpty() const {
    
    return m_size == 0;
}

template <typename KeyType, typename ValueType, typename HashType>
std::size_t HashDictionary<KeyType, ValueType, HashType>::size() const {
    
    return m_size;
}

template <typename KeyType, typename ValueType, typename HashType>
void HashDictionary<KeyType, ValueType, HashType>::add(const KeyType &key,
                                                       const ValueType &value)
{
    // TODO implement the add method...
    
    // 1. hash the key
    std::size_t index = m_hash(key) % m_capacity;
    
    // 2. do linear probing
    std::size_t numprobes = 0;
    while (m_data[index].filled && (numprobes < m_capacity)) 
    {
        if (m_data[index].key == key) 
        {
            //if key already exists, add the item
            m_data[index].value = value;
            return;
        }
        index = (index + 1) % m_capacity;
        numprobes+=1;
    }
    // 3. Check to see if linear probing has failed
        if (numprobes == m_capacity) 
        {
            throw std::logic_error("Too many probes in HashDictionary::add");
        }
    // 4. insert the key-value pair
        m_data[index].filled = true;
        m_data[index].key = key;
        m_data[index].value = value;
        m_size++;
    // 5. test if we need to reallocate��and reallocate if needed

        if (static_cast<float>(m_size) / m_capacity > m_load_factor)
        {
            //increase the capacity
            std::size_t new_capacity = m_capacity +1000;

            // create a new data array 
            KeyValueType* new_data = new KeyValueType[new_capacity];

            //move the old item to new data 
            for (std::size_t i = 0; i < m_capacity; i++) 
            {
                if (m_data[i].filled) 
                {
                    // rehash the key in the new array.
                    std::size_t new_index = m_hash(m_data[i].key) % new_capacity;
                    
                    //while array[index] is full, 
                    while (new_data[new_index].filled) 
                    {
                        new_index = (new_index + 1) % new_capacity;
                    }
                    new_data[new_index] = m_data[i];
                }
            }
            //delete the old one and update the new one
            delete[] m_data;
            m_data = new_data;
            m_capacity = new_capacity;

        }
      

}

template <typename KeyType, typename ValueType, typename HashType>
void HashDictionary<KeyType, ValueType, HashType>::remove(const KeyType &key) {
    
    //TODO implement the remove method...

    // hash the key
    std::size_t index = m_hash(key) % m_capacity;

    //do linear probing
    std::size_t numprobes = 0;
    while (m_data[index].filled && numprobes < m_capacity) {
        if (m_data[index].key == key) {
            //found the key , then remove it
            m_data[index].filled = false;
            m_size--;
            return;
        }
        index = (index + 1) % m_capacity;
        numprobes++;
    }

    //if not found
    throw std::logic_error("Not Found the key in HashDictionary::remove");
}

template <typename KeyType, typename ValueType, typename HashType>
void HashDictionary<KeyType, ValueType, HashType>::clear() {
    
    m_size = 0;
    for (std::size_t i = 0; i < m_capacity; ++i) {
        m_data[i].filled = false;
    }
}

template <typename KeyType, typename ValueType, typename HashType>
ValueType
HashDictionary<KeyType, ValueType, HashType>::get(const KeyType &key) const
{
    
    // hash the key
    std::size_t index = m_hash(key) % m_capacity;
    
    // do linear probing
    std::size_t numprobes = 0;
    while (m_data[index].filled && (numprobes < m_capacity)) {
        if (m_data[index].key == key) {
            break;
        }
        index = (index + 1) % m_capacity;
        numprobes += 1;
    }
    if (numprobes == m_capacity) {
        throw std::logic_error("Too many probes in HashDictionary::get");
    }
    
    if (!m_data[index].filled) {
        throw std::logic_error("Nonexistant key in HashDictionary::get");
    }
    
    return m_data[index].value;
}

template <typename KeyType, typename ValueType, typename HashType>
bool HashDictionary<KeyType, ValueType, HashType>::contains(
                                                            const KeyType &key) const
{
    // hash the key
    std::size_t index = m_hash(key) % m_capacity;
    
    // do linear probing
    std::size_t numprobes = 0;
    while (m_data[index].filled && (numprobes < m_capacity)) {
        if (m_data[index].key == key) {
            break;
        }
        index = (index + 1) % m_capacity;
        numprobes += 1;
    }
    if (numprobes == m_capacity) {
        throw std::logic_error("Too many probes in HashDictionary::contains");
    }
    
    return m_data[index].filled;
}
