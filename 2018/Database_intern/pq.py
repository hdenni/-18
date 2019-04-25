import heapq
class PriorityQueue:
    pq = []
    elements = {}
    counter = 0
    
    def insert(self, priority, value):
        if value in self.elements:
            if priority < self.elements[value][0]:
                self.delete(value)
            else:
                return
        entry = [priority, self.counter, value]
        self.counter += 1
        self.elements[value] = entry
        heapq.heappush(self.pq, entry)
        
    def delete(self, value):
        entry = self.elements[value]
        entry[-1] = None
        
    def pop(self):
        while self.pq:
            priority, counter, value = heapq.heappop(self.pq)
            if value != None:
                del self.elements[value]
                return priority, value
        raise KeyError('Pop from an empty PriorityQueue')    
    
    def size(self):
        return len(self.elements)