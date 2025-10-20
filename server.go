package main

import (
	"net/http"
	"strconv"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
)

type ListType struct {
	ID         int    `json:"id"`
	DISPLAY    string `json:"display"	`
	R          int    `json:"r"`
	G          int    `json:"g"`
	B          int    `json:"b"`
	BRIGHTNESS int    `json:"brightness"`
	COLOR      string `json:"color"`
}

var List = []ListType{
	{ID: 1, DISPLAY: "normal", R: 255, G: 0, B: 0, BRIGHTNESS: 20, COLOR: "red"},
}

func getList(c *gin.Context) {
	c.IndentedJSON(http.StatusOK, List)
}

func getByID(c *gin.Context) {
	idParam := c.Param("id")
	id, err := strconv.Atoi(idParam)

	if err != nil {
		c.IndentedJSON(http.StatusBadRequest, gin.H{"meesage": "Id is not exit"})
	}

	for i, t := range List {
		if t.ID == id {
			c.IndentedJSON(http.StatusOK, List[i])
			return
		}
	}
}

func PatchList(c *gin.Context) {
	var newList ListType

	if err := c.BindJSON(&newList); err != nil {
		c.IndentedJSON(http.StatusBadRequest, gin.H{"message": "Error"})
		return
	}

	for i, t := range List {
		if t.ID == newList.ID {
			List[i].COLOR = newList.COLOR
			List[i].DISPLAY = newList.DISPLAY
			List[i].R = newList.R
			List[i].G = newList.G
			List[i].B = newList.B
			List[i].BRIGHTNESS = newList.BRIGHTNESS

			c.IndentedJSON(http.StatusOK, List)
			return
		}
	}
}

func main() {
	router := gin.Default()
	router.Use(cors.New(cors.Config{
		AllowOrigins:     []string{"http://localhost:5173", "http://192.168.100.105:8000"},
		AllowMethods:     []string{"GET", "POST", "PATCH"},
		AllowHeaders:     []string{"Content-Type"},
		AllowCredentials: true,
	}))
	router.GET("/List", getList)
	router.GET("/List/:id", getByID)
	router.PATCH("/List", PatchList)
	router.Run("192.168.100.105:8000")
}
