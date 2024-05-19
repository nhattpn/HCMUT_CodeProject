library(dplyr)
library(stringr)
library("tidyverse")
library(corrplot)
library(datarium)
library(Hmisc)
library(psych)
library(ggplot2)
library(datasets)
#-------------Đọc dữ liệu ----------------------------------------
cpus <- read.csv("Intel_CPUs.csv", header = TRUE)


missing_values <- sapply(cpus, function(x) sum((is.na(x) + (x == ""))/length(x)))

barplot(missing_values,
        names.arg = names(cpus),
        col = "red",
        main = "Tỉ lệ Dữ liệu Mất trong Từng Biến",
        xlab = "",
        ylab = "Tỉ lệ Dữ liệu Mất",
        las = 2,  # Xoay nhãn trục x 90 độ
        cex.names = 0.4,  # Giảm kích thước chữ của nhãn
        space = 1,
        ylim = c(0, 1 + 0.1))  # Tăng tỉ lệ trục y tối đa lên 1

mtext("Biến", side = 1, line = 4, cex = 0.6)  # Thêm nhãn cho trục x

grid(nx = NA, ny = NULL)


#-------------Hàm xử lý số liệu-----------------------------------
PriceProcessor <- function(x) {
  x <- gsub(",", "", x)
  matches <- regmatches(x, gregexpr("\\$([0-9]+(\\.[0-9]+)?)", x))
  if (any(grepl("-", x))) {
    values <- as.numeric(gsub("\\$", "", unlist(matches)))
    if (length(values) >= 2) {
      ans <- mean(values, na.rm = TRUE)
    } else {
      ans <- NA
    }
  } else if (length(matches[[1]]) > 0) {
    ans <- as.numeric(sub("\\$", "", matches[[1]][1]))
  } else {
    ans <- NA
  }
  return(ans)
}
get_numbers <- function(word) {
  if (is.character(word)) {
    return(as.numeric(str_extract(word, "[\\d]*[.]?[\\d]+")))
  } else {
    return(word)
  }
}
CacheMapper = function(x) {
  if (is.numeric(x)){
    return(x)
  } else if (grepl("K", x)){
    fac <- 1000
  } else if (grepl("M", x)){
    fac <- 1000000
  } else if (grepl("G", x)){
    fac <- 1000000000
  } else if (grepl("T", x)){
    fac <- 1000000000000} else {
      fac <- 1 
    }
  return(fac*get_numbers(x))
}
L_Date = function(x) {
  if (!grepl("^Q[1-4]'\\d{2}$", x)) {
    return(NA)
  }
  year <- as.numeric(substr(x, 4, 5))
  quarter_number <- as.numeric(substr(x, 2, 2))
  
  if(year > 24) {
    year <- year + 1900
  }
  else {
    year <- year + 2000
  }
  if (quarter_number == 1) {
    year <- year
  } else if(quarter_number == 2){
    year <- year + 0.25
  } else if(quarter_number == 3){
    year <- year + 0.5
  } else {
    year <- year + 0.75
  }
  return (year)
}
ProcessorMapper = function(x) {
  value <- as.double(as.numeric(substr(x, 1, nchar(x) - 4)))
  if (substr(x, nchar(x) - 2, nchar(x) - 2) == "G") {
    value <- value*1000000000
  } else if (substr(x, nchar(x)-2, nchar(x)-2) == "M"){
    value <- value*1000000
  } else if (substr(x, nchar(x)-2, nchar(x)-2) == "K"){
    value <- value*1000
  } else if (substr(x, nchar(x)-2, nchar(x)-2) == "T") {
    value <- value*1000000000000
  }
  
  return(value)
}
Cleanify = function(tag, func, df) {
  clean_df <- df[!is.na(df[[tag]]), , drop = FALSE]
  clean_df <- data.frame(clean_df)
  rownames(clean_df) <- NULL
  clean_df[[tag]] <- lapply(clean_df[[tag]], func)
  return(clean_df)
}
convert_bus_speed <- function(speed) {
  if (grepl("GT/s", speed)) {
    numeric_speed <- as.numeric(sub(" GT/s.*", "", speed))
    return(numeric_speed * 10^9)
  } else if (grepl("MHz", speed)) {
    numeric_speed <- as.numeric(sub(" MHz.*", "", speed))
    return(numeric_speed * 10^6)
  } else {
    return(NA)
  }
}
convert_mm_size <- function(mm) {
  # Tách số và đơn vị từ chuỗi
  number <- as.numeric(gsub("[^0-9.]", "", mm))
  unit <- gsub("[0-9.\\s]", "", mm)
  
  # Chuyển đổi số dựa trên đơn vị
  
  if (!is.na(unit)) {
    if (toupper(unit) == " GB") {
      return (number)
    } else {
      return (number * 1024)
    }
  }
  else {
    return (NA)
  }
}
convert_mm_banwith <- function(mm_b) {
  if (grepl("GB/s", mm_b)) {
    numeric_speed <- as.numeric(sub(" GB/s.*", "", mm_b))
    return(numeric_speed)
  } else if (grepl("TB/s", mm_b)) {
    numeric_speed <- as.numeric(sub(" TB/s*", "", mm_b))
    return(numeric_speed * 1024)
  } else {
    return(NA)
  }
}


#-------------Xử lý số liệu ----------------------------------

#-------------Xóa các cột có Processor_Number = 0-------------
cpus <- cpus[cpus$Processor_Number != "", ]
#-------------Product_Collection--------------------------------------
cpus <- cpus %>%
  mutate(Product_Collection = gsub('.*Core.*', 'Core', Product_Collection),
         Product_Collection = gsub('.*X-series.*', 'X-series', Product_Collection),
         Product_Collection = gsub('.*Celeron.*', 'Celeron', Product_Collection),
         Product_Collection = gsub('.*Pentium.*', 'Pentium', Product_Collection),
         Product_Collection = gsub('.*Quark.*', 'Quark', Product_Collection),
         Product_Collection = gsub('.*Core. [mM].*', 'm', Product_Collection),
         Product_Collection = gsub('.*Atom.*', 'Atom', Product_Collection),
         Product_Collection = gsub('.*Itanium.*', 'Itanium', Product_Collection),
         Product_Collection = gsub('.*Xeon.*', 'Xeon', Product_Collection))

#-------------Price-----------------------
cpus$Recommended_Customer_Price <- sapply(cpus$Recommended_Customer_Price, PriceProcessor)
cpus$Recommended_Customer_Price <- as.numeric(cpus$Recommended_Customer_Price)
#-------------cpus_need_prediction------------
cpus_need_prediction <- filter(cpus, is.na(Recommended_Customer_Price))
#-------------Xóa các cột Recommended_Customer_Price không có giá trị ------------------------------
cpus <- cpus[!is.na(cpus$Recommended_Customer_Price), ]
#-------------Lithography------------------
cpus$Lithography <- gsub(" nm", '', cpus$Lithography)
cpus$Lithography <- as.numeric(cpus$Lithography)
#-------------Cache--------------------------
cpus$Cache <- sapply(cpus$Cache, CacheMapper)
#-------------Launch_Date-------------------------------------------------------------------------
cpus$Launch_Date <- sapply(cpus$Launch_Date, L_Date)
cpus$Launch_Date <- as.numeric(cpus$Launch_Date)
#-------------Memory-size----------------
cpus$Max_Memory_Size <- sapply(cpus$Max_Memory_Size, convert_mm_size)
#-------------Bus-speed------------------
cpus$Bus_Speed <- sapply(cpus$Bus_Speed, convert_bus_speed)
#--------Đọc dữ liệu vào df_cpus-------------------------------------------------------
df_cpus <- cpus[,c("Product_Collection", "Launch_Date", "Recommended_Customer_Price", "Lithography"
                   , "Cache" , "nb_of_Threads" , "Bus_Speed" 
                   , "Max_Memory_Size")]
#--------Kiểm tra dữ liệu khuyết-------------------------------------------------------
apply(is.na(df_cpus), 2, sum)
apply(is.na( df_cpus) ,2, which)
apply (is.na( df_cpus ),2, mean)
#--------Thay thế dữ liệu khuyết bởi mean-------------------------------------------------------

df_cpus$Lithography[is.na(df_cpus$Lithography)] = mean(df_cpus$Lithography, na.rm = TRUE)

df_cpus$nb_of_Threads[is.na(df_cpus$nb_of_Threads)] = mean(df_cpus$nb_of_Threads, na.rm = TRUE)

df_cpus$Cache[is.na(df_cpus$Cache)] = mean(df_cpus$Cache, na.rm = TRUE)

df_cpus$Launch_Date[is.na(df_cpus$Launch_Date)] = mean(df_cpus$Launch_Date, na.rm = TRUE)

df_cpus$Bus_Speed <- as.numeric(df_cpus$Bus_Speed)
df_cpus$Bus_Speed[is.na(df_cpus$Bus_Speed)] <- mean(df_cpus$Bus_Speed, na.rm = TRUE)

df_cpus$Max_Memory_Size <- as.numeric(df_cpus$Max_Memory_Size)
df_cpus$Max_Memory_Size[is.na(df_cpus$Max_Memory_Size)] <- mean(df_cpus$Max_Memory_Size, na.rm = TRUE)
#--------Kiểm tra lại dữ liệu khuyết-------------------------------------------------------
apply(is.na(df_cpus), 2, sum)
apply(is.na( df_cpus) ,2, which)
apply (is.na( df_cpus) ,2, mean)
#------------------THỐNG KÊ MÔ TẢ------------------------------------------------------------------------------
summary(df_cpus)
#-------Hàm xử lý thống kê mô tả---------------
histogram <- function(x, color){
  x_name = deparse(substitute(x))
  x_name <- gsub(".*\\$", "", x_name)
  distance <- (max(x)-min(x)) * 0.05
  ggplot(data = data.frame(value = x), aes(x = value)) +
    geom_histogram(binwidth = distance, fill = color, color = "black") + 
    labs(title = paste("Histogram of ", x_name), x = "", y = "Frequency", ) + 
    theme(plot.title = element_text(size = 50, hjust = 0.5), 
          axis.title.y = element_text(size=45), 
          axis.text.x = element_text(size =40), 
          axis.text.y = element_text(size=40))
}

scatterplot <- function(x, color){
  x_name = deparse(substitute(x))
  x_name <- gsub(".*\\$", "", x_name)
  distance <- floor((max(x)-min(x)) * 0.33)
  y = cpus$Recommended_Customer_Price
  y_name = "Recommended_Customer_Price"
  ggplot(data = data.frame(x = x, y = y), aes(x = x, y =y)) +
    geom_point(size = 3, color = color) + 
    labs(title = paste("Scatter Plot of ", x_name), x = "", y = "Frequency", ) + 
    theme(plot.title = element_text(size = 45, hjust = 0.5), 
          axis.title.y = element_text(size=40), 
          axis.text.x = element_text(size =30), 
          axis.text.y = element_text(size=30)) + 
    scale_x_continuous(breaks = seq(min(x), max(x), by = distance)) 
}

boxPlot <- function(x, color) {
  x_name = deparse(substitute(x))
  x_name <- gsub(".*\\$", "", x_name)
  distance <- floor((max(x)-min(x)) * 0.33)
  ggplot(data = data.frame(value = x), aes(y = value)) +
    geom_boxplot(fill = color, color = "black") +
    labs(title = paste("Boxplot of ", x_name), x = "", y = "") +
    theme(plot.title = element_text(size = 40, hjust = 0.5), 
          axis.title.y = element_text(size=40),  
          axis.text.y = element_text(size=30)) + 
    scale_y_continuous(breaks = seq(floor(min(x)), max(x), by = distance)) 
}
#------Biễu diễn histogram----------
histogram(df_cpus$Launch_Date, "red")
histogram(df_cpus$Recommended_Customer_Price, "green")
histogram(df_cpus$Lithography, "gray")
histogram(df_cpus$Cache, "violet")
histogram(df_cpus$nb_of_Threads, "blue")
histogram(df_cpus$Bus_Speed, "salmon")
histogram(df_cpus$Max_Memory_Size, "brown")
#--------Biểu diễn scatter plot--------
scatterplot(df_cpus$Launch_Date, "red")
scatterplot(df_cpus$Lithography, "gray")
scatterplot(df_cpus$Cache, "violet")
scatterplot(df_cpus$nb_of_Threads, "blue")
scatterplot(df_cpus$Bus_Speed, "salmon")
scatterplot(df_cpus$Max_Memory_Size, "brown")
#--------Biểu diễn box plot---------
boxPlot(df_cpus$Launch_Date, "red")
boxPlot(df_cpus$Recommended_Customer_Price, "green")
boxPlot(df_cpus$Lithography, "gray")
boxPlot(df_cpus$Cache, "violet")
boxPlot(df_cpus$nb_of_Threads, "blue")
boxPlot(df_cpus$Bus_Speed, "salmon")
boxPlot(df_cpus$Max_Memory_Size, "brown")


###############THỐNG KÊ SUY DIỄN##################
###Chia dữ liệu thành tập train_data để xây dựng mô hình và tập test_data để kiểm tra mô hình
par(mfrow = c(1,3))
plot(df_cpus$Launch_Date, df_cpus$Recommended_Customer_Price, pch = 16, col = 'blue', main = "Launch_Date")
plot(df_cpus$Lithography, df_cpus$Recommended_Customer_Price, pch = 16, col = 'blue', main = "Lithography")
plot(df_cpus$nb_of_Threads, df_cpus$Recommended_Customer_Price, pch = 16, col = 'blue', main = "nb_of_Threads")
plot(df_cpus$Cache, df_cpus$Recommended_Customer_Price, pch = 16, col = 'blue', main = "Cache")
plot(df_cpus$Bus_Speed, df_cpus$Recommended_Customer_Price, pch = 16, col = 'blue', main = "Bus_Speed")
plot(df_cpus$Max_Memory_Size, df_cpus$Recommended_Customer_Price, pch = 16, col = 'blue', main = "Max_Memory_Size")

#Mô hình gốc: đưa tất cả biến độc lập vào mô hình
model_Original <- lm(Recommended_Customer_Price ~ Launch_Date+Lithography+nb_of_Threads+Cache+Bus_Speed+Max_Memory_Size, data = df_cpus)
summary(model_Original)

### Cải thiện các mô hình

#Mô hình cải thiện: Bỏ biến Bus_Speed ra khỏi mô hình gốc
model_fix <- lm(Recommended_Customer_Price ~ Launch_Date+Lithography+nb_of_Threads+Cache+Max_Memory_Size, data = df_cpus)
summary(model_fix)

###->chọn mô hình gốc vì không có mô hình cải thiện nào tốt hơn
###dùng lệnh plot(model_Original) để vẽ các biểu đồ kiểm định mô hình

### Kiểm tra các giả định
## Giả định về tính tuyến tính của mô hình và tính độc lập của phương sai (Residuals vs Fitted)
plot(model_Original, 1)
## Giả định về phân phối chuẩn của các sai số (Normal-QQ)
plot(model_Original, 2)
## Giả định về tính bất biến của phương sai, phương sai là hằng số (Scale - Location)
plot(model_Original, 3)
## Xác định những điểm có ảnh hưởng cao.
plot(model_Original, 5)

### Hồi quy stepwise
stepwise_original<-step(model_Original, direction="backward")
stepwise_fix<-step(model_fix, direction="backward")

### Dự đoán giá
predicted_Price <- predict(model_Original, newdata = df_cpus)
predicted_df <- data.frame(df_cpus, Predicted_Price = predicted_Price)

ggplot(predicted_df, aes(x = Recommended_Customer_Price, y = Predicted_Price)) +
  geom_point() +  # Điểm biểu diễn các giá trị
  geom_abline(intercept = 0, slope = 1, color = "red") +  # Đường thẳng y = x
  labs(x = "Recommended Customer Price", y = "Predicted Price", title = "Actual vs. Predicted Price") +
  theme_minimal()

###Kiểm tra giá trị MSE
range_of_Price <- range(df_cpus$Recommended_Customer_Price)
print(range_of_Price)
MSE = sqrt(mean((predicted_Price - df_cpus$Recommended_Customer_Price)^2))
print(MSE)


