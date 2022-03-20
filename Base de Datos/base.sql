-- MySQL dump 10.13  Distrib 8.0.28, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: monitorplantas
-- ------------------------------------------------------
-- Server version	8.0.28

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `cam`
--

DROP TABLE IF EXISTS `cam`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `cam` (
  `idCAM` int DEFAULT NULL,
  `fechaCAM` date DEFAULT NULL,
  `estatus` varchar(25) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cam`
--

LOCK TABLES `cam` WRITE;
/*!40000 ALTER TABLE `cam` DISABLE KEYS */;
/*!40000 ALTER TABLE `cam` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dhthum`
--

DROP TABLE IF EXISTS `dhthum`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `dhthum` (
  `idHum` int DEFAULT NULL,
  `fechaHum` date DEFAULT NULL,
  `horaHum` time DEFAULT NULL,
  `lecHumDHT` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dhthum`
--

LOCK TABLES `dhthum` WRITE;
/*!40000 ALTER TABLE `dhthum` DISABLE KEYS */;
/*!40000 ALTER TABLE `dhthum` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dhttemp`
--

DROP TABLE IF EXISTS `dhttemp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `dhttemp` (
  `idTemp` int DEFAULT NULL,
  `fechaTemp` date DEFAULT NULL,
  `horaTemp` time DEFAULT NULL,
  `lecTempDHT` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dhttemp`
--

LOCK TABLES `dhttemp` WRITE;
/*!40000 ALTER TABLE `dhttemp` DISABLE KEYS */;
/*!40000 ALTER TABLE `dhttemp` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `humt`
--

DROP TABLE IF EXISTS `humt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `humt` (
  `idT` int DEFAULT NULL,
  `fechaT` date DEFAULT NULL,
  `horaT` time DEFAULT NULL,
  `lecT` varchar(7) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `humt`
--

LOCK TABLES `humt` WRITE;
/*!40000 ALTER TABLE `humt` DISABLE KEYS */;
/*!40000 ALTER TABLE `humt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `registro`
--

DROP TABLE IF EXISTS `registro`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `registro` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `usuario` varchar(20) DEFAULT NULL,
  `planta` varchar(150) DEFAULT NULL,
  `idHum` int DEFAULT NULL,
  `idTemp` int DEFAULT NULL,
  `idUV` int DEFAULT NULL,
  `IdCAM` int DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `registro`
--

LOCK TABLES `registro` WRITE;
/*!40000 ALTER TABLE `registro` DISABLE KEYS */;
/*!40000 ALTER TABLE `registro` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `uv`
--

DROP TABLE IF EXISTS `uv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `uv` (
  `idUV` int DEFAULT NULL,
  `fechaUV` date DEFAULT NULL,
  `horaUV` time DEFAULT NULL,
  `lecUV` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `uv`
--

LOCK TABLES `uv` WRITE;
/*!40000 ALTER TABLE `uv` DISABLE KEYS */;
/*!40000 ALTER TABLE `uv` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-03-06 22:09:16
