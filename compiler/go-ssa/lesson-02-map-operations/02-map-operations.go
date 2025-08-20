package demo

func getFromMap(m map[string]int, key string) int {
	value, exists := m[key]
	if exists {
		return value
	}
	return -1
}

func setToMap(m map[string]int, key string, value int) {
	m[key] = value
}

func main() {
	// 创建map
	scores := make(map[string]int)
	
	// 设置值
	setToMap(scores, "alice", 95)
	setToMap(scores, "bob", 87)
	
	// 获取值
	aliceScore := getFromMap(scores, "alice")
	charlieScore := getFromMap(scores, "charlie") // 不存在的key
	
	println("Alice's score:", aliceScore)
	println("Charlie's score:", charlieScore)
}