#include "list.hpp"

#include <utility>
#include <algorithm>

namespace pjc {
    list::~list() {
        while(head != nullptr){
            auto n = head->next;
            delete head;
            head = n;
        }
    }

    list::list(const std::vector<double> &vec) {
        for(auto el : vec){
            push_back(el);
        }
    }

    list::list(const pjc::list &rhs) {
        for(auto ele : rhs){
            push_back(ele);
        }
    }

    bool list::empty() const {
        return num_elements == 0;
    }

    size_t list::size() const {
        return num_elements;
    }

    void list::push_back(double elem) {
        const auto& n = new list::node();
        n->val= elem;
        n->prev = tail;
        if(tail != nullptr){
            tail->next = n;
        }
        tail = n;
        if(head == nullptr){
            head = tail;
        }
        num_elements++;
    }

    void list::pop_back() {
        if(tail->prev == nullptr){
            delete tail;
            tail = nullptr;
            head = nullptr;
            num_elements--;
            return;
        }
        tail = tail->prev;
        delete tail->next;
        tail->next = nullptr;
        num_elements--;
    }

    void list::push_front(double elem) {
        auto n = new node;
        n->val = elem;
        n->next = head;
        if(head != nullptr) head->prev = n;
        head = n;
        if(tail == nullptr) tail = head;
        num_elements++;
    }

    void list::pop_front() {
        if(head->next == nullptr){
            delete head;
            head = nullptr;
            tail = nullptr;
            num_elements--;
            return;
        }
        head = head->next;
        delete head->prev;
        head->prev = nullptr;
        num_elements--;
    }

    double const& list::back() const {
        return tail->val;
    }

    double& list::back() {
        return tail->val;
    }

    double const& list::front() const {
        return head->val;
    }

    double& list::front() {
        return head->val;
    }

    bool list::operator==(const pjc::list &rhs) const {
        if(size() != rhs.size()) return false;
        auto curr = begin();
        for(const auto& x : rhs){
            if(*curr != x) return false;
            curr++;
        }
        return true;
    }

    list::iterator::iterator(pjc::list::node *ptr, const pjc::list *gen) {
        current_ptr = ptr;
        o_list = gen;
    }

    list::iterator list::begin() {
        return {head, this};
    }

    list::iterator list::end() {
        return {nullptr, this};
    }

    list::iterator& list::iterator::operator++() {
        current_ptr = current_ptr->next;
        return *this;
    }

    list::iterator& list::iterator::operator--() {
        if(current_ptr == nullptr){
            current_ptr = o_list->tail;
        } else {
            current_ptr = current_ptr->prev;
        }
        return (*this);
    }

    bool list::iterator::operator==(const pjc::list::iterator &rhs) const {
        return current_ptr == rhs.current_ptr && o_list == rhs.o_list;
    }

    bool list::iterator::operator!=(const pjc::list::iterator &rhs) const {
        return current_ptr != rhs.current_ptr || o_list != rhs.o_list;
    }

    double & list::iterator::operator*() const {
        return current_ptr->val;
    }

    list::iterator list::iterator::operator++(int) {
        auto prev = current_ptr;
        current_ptr = current_ptr->next;
        return {prev, o_list};
    }

    list::iterator list::iterator::operator--(int) {
        auto prev = current_ptr;
        if(current_ptr == nullptr){
            current_ptr = o_list->tail;
        } else {
            current_ptr = current_ptr->prev;
        }
        return {prev, o_list};
    }

    list::iterator::operator const_iterator() const {
        return {
            current_ptr,
            o_list
        };
    }

    list::iterator::pointer list::iterator::operator->() const {
        return &current_ptr->val;
    }

    list::const_iterator::const_iterator(pjc::list::node *ptr, const pjc::list *gen) {
        current_ptr = ptr;
        o_list = gen;
    }

    list::const_iterator list::begin() const {
        return {head, this};
    }

    list::const_iterator list::end() const {
        return {nullptr, this};
    }

    list::const_iterator& list::const_iterator::operator++() {
        current_ptr = current_ptr->next;
        return (*this);
    }

    list::const_iterator& list::const_iterator::operator--() {
        if(current_ptr == nullptr){
            current_ptr = o_list->tail;
        } else {
            current_ptr = current_ptr->prev;
        }
        return (*this);
    }

    bool list::const_iterator::operator==(const pjc::list::const_iterator &rhs) const {
        return current_ptr == rhs.current_ptr && o_list == rhs.o_list;
    }

    bool list::const_iterator::operator!=(const pjc::list::const_iterator &rhs) const {
        return current_ptr != rhs.current_ptr || o_list != rhs.o_list;
    }

    const double & list::const_iterator::operator*() const {
        return current_ptr->val;
    }

    list::const_iterator list::const_iterator::operator++(int) {
        auto prev = current_ptr;
        current_ptr = current_ptr->next;
        return {prev, o_list};
    }

    list::const_iterator list::const_iterator::operator--(int) {
        auto prev = current_ptr;
        if(current_ptr == nullptr){
            current_ptr = o_list->tail;
        } else {
            current_ptr = current_ptr->prev;
        }
        return {prev, o_list};
    }

    list::const_iterator::pointer list::const_iterator::operator->() const {
        return &current_ptr->val;
    }

    list::const_iterator list::cend() const {
        return {nullptr, this};
    }

    list::const_iterator list::cbegin() const {
        return {head, this};
    }

    list& list::operator=(const pjc::list &rhs) {
        if(&rhs == this) return *this;
        while(!empty())pop_front();
        for(auto x : rhs){
            push_back(x);
        }
        return *this;
    }

    list& list::operator=(pjc::list &&rhs) {
        while(!empty())pop_front();
        head = rhs.head;
        tail = rhs.tail;
        num_elements = rhs.num_elements;
        rhs.head = nullptr;
        rhs.tail = nullptr;
        rhs.num_elements = 0;
        return (*this);
    }

    list::list(pjc::list &&rhs) {
        head = rhs.head;
        tail = rhs.tail;
        num_elements = rhs.num_elements;
        rhs.head = nullptr;
        rhs.tail = nullptr;
        rhs.num_elements = 0;
    }

    bool list::operator<(const pjc::list &rhs) const {
        if(size() == 0 && rhs.size() > 0) return true;
        if(size() == 0 && rhs.size() == 0) return false;
        auto curr = begin();
        for(const auto& x : rhs){
            if(curr == end()) return true;
            if((*curr) < x) return true;
            if((*curr) > x) return false;
            curr++;
        }
        return false;
    }

    bool operator!=(const list &lhs, const list &rhs) {
        if(lhs.size() != rhs.size()) return true;
        auto curr = lhs.begin();
        for(const auto& x : rhs){
            if(*curr != x) return true;
            curr++;
        }
        return false;
    }

    bool operator>(const list &lhs, const list &rhs) {
        if(lhs.size() == rhs.size() == 0) return false;
        if(lhs.size() == 0) return false;
        auto curr = lhs.begin();
        for(const auto& x : rhs){
            if(*curr > x) return true;
            if(*curr < x) return false;
        }
        return lhs.size() < rhs.size();
    }

    bool operator<=(const list &lhs, const list &rhs) {
        return !(lhs > rhs);
    }

    bool operator>=(const list &lhs, const list &rhs) {
        return !(lhs < rhs);
    }

    std::pair<list, list> list::split(list::const_iterator place) {
        auto ret = std::pair<list, list>();
        list::const_iterator itr = begin();
        while(itr != place){
            ret.first.push_back(*itr);
            itr++;
            pop_front();
        }
        ret.second.head = place.current_ptr;
        ret.second.tail = tail;
        ret.second.num_elements = num_elements;
        tail = nullptr;
        head = nullptr;
        num_elements = 0;
        return ret;
    }

    void list::merge(pjc::list &rhs) {
        if(empty()){
            while(!rhs.empty()){
                push_back(rhs.front());
                rhs.pop_front();
            }
        }
        if(rhs.empty())
            return;

        auto hl = head;
        while(!rhs.empty()){
            if(hl->val <= rhs.front()){
                if(hl->next != nullptr){
                    hl = hl->next;
                } else {
                   while(!rhs.empty()){
                       push_back(rhs.front());
                       rhs.pop_front();
                   }
                   break;
                }
            } else {
                auto toAdd = rhs.front();
                rhs.pop_front();
                auto n = new node;
                n->val = toAdd;
                if(hl->prev != nullptr){
                    hl->prev->next = n;
                } else {
                    head = n;
                }
                n->prev = hl->prev;
                n->next = hl;
                hl->prev = n;
                num_elements++;
            }
        }
        while(!rhs.empty())rhs.pop_front();
    }

    void list::reverse() {
        if(size() == 0) return;
        auto l = head, r = tail;
        while(head != tail){
            auto tmp = r->val;
            r->val = l->val;
            l->val = tmp;
            r = r->prev;
            if(r == l) break;
            l = l->next;
        }
    }

    void list::remove(double value) {
        if(size() == 0)
            return;

        // delete from front until first valid element
        while(head->val == value){
            if(head->next == nullptr){
                delete head;
                head = nullptr;
                tail = nullptr;
                num_elements--;
                return;
            } else {
                head = head->next;
                delete head->prev;
                num_elements--;
            }
        }

        // delete from back until first valid element
        while(tail->val == value){
            tail = tail->prev;
            delete tail->next;
            tail->next = nullptr;
            num_elements--;
        }

        auto curr = head->next;
        while(curr != tail && curr != nullptr){
            if(curr->val == value){
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                num_elements--;
                auto n = curr;
                curr = curr->next;
                delete n;
                continue;
            } else {
                curr = curr->next;
            }
        }
    }

    void list::sort() {

    }

    void list::swap(list &rhs) {
        std::swap(*this, rhs);
    }

} // end namespace pjc
