#ifndef LinkedList_H
#define LinkedList_H

#include <functional>
#include <stdexcept>

template<typename T>
class Node {
public:
    T data;
    Node* next;
    Node* prev;

    Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

template<typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~LinkedList() {
        clear();
    }

    void append(const T& value) {
        Node<T>* newNode = new Node<T>(value);

        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }


    void prepend(const T& value) {
        Node<T>* newNode = new Node<T>(value);

        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    bool removeAt(int index) {
        if (index < 0 || index >= size) return false;

        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }

        if (current->prev) {
            current->prev->next = current->next;
        } else {
            head = current->next;
        }

        if (current->next) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev;
        }

        delete current;
        size--;
        return true;
    }

    T& at(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    const T& at(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }


    LinkedList<T> filter(std::function<bool(const T&)> predicate) const {
        LinkedList<T> result;
        Node<T>* current = head;

        while (current) {
            if (predicate(current->data)) {
                result.append(current->data);
            }
            current = current->next;
        }

        return result;
    }


    void forEach(std::function<void(T&)> func) {
        Node<T>* current = head;
        while (current) {
            func(current->data);
            current = current->next;
        }
    }

    void forEach(std::function<void(const T&)> func) const {
        Node<T>* current = head;
        while (current) {
            func(current->data);
            current = current->next;
        }
    }

    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }


    class Iterator {
    private:
        Node<T>* current;
    public:
        Iterator(Node<T>* node) : current(node) {}

        T& operator*() { return current->data; }
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

#endif 
