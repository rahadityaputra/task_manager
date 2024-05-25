-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: May 25, 2024 at 01:46 PM
-- Server version: 10.4.28-MariaDB
-- PHP Version: 8.2.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `task_manager_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `task`
--

CREATE TABLE `task` (
  `task_id` int(11) NOT NULL,
  `task_name` varchar(50) NOT NULL,
  `description` varchar(1000) NOT NULL,
  `deadline` datetime NOT NULL,
  `created_at` datetime NOT NULL,
  `user_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `task`
--

INSERT INTO `task` (`task_id`, `task_name`, `description`, `deadline`, `created_at`, `user_id`) VALUES
(1, 'Belajar html', 'belajar html dengan youtube wpu', '2024-05-23 00:00:00', '2024-05-21 16:52:31', 19),
(2, 'pr algo', 'setelah pulang sekolah langsung kerjain', '2024-05-22 00:00:00', '2024-05-21 16:56:20', 19),
(10, 'beli makan', 'beli makan di dekat masjid', '2024-05-22 00:00:00', '2024-05-21 16:58:43', 20),
(12, 'membuat cilor', 'membuat cilor dengan bahan telur dan makaroni yang direbus terlebih dahulu', '0000-00-00 00:00:00', '2024-05-24 15:58:01', 19),
(13, 'mengerjakan projek alpro', 'memembuat algoritma nya terlebih dahulu ', '2024-05-24 00:00:00', '2024-05-24 16:04:03', 19),
(14, 'mencuci baju', '', '2024-05-25 00:00:00', '2024-05-25 10:25:30', 19),
(15, 'membeli buku', '', '2024-05-25 00:00:00', '2024-05-25 16:39:49', 19),
(16, 'garap proejk', 'wkwkwk', '0000-00-00 00:00:00', '2024-05-25 18:17:42', 19),
(17, 'ke rumah temen', '', '0000-00-00 00:00:00', '2024-05-25 18:19:00', 20),
(18, 'presentasi tugas algo', 'jam 12 di ruang patt 3 c', '0000-00-00 00:00:00', '2024-05-25 18:23:11', 20);

-- --------------------------------------------------------

--
-- Table structure for table `user`
--

CREATE TABLE `user` (
  `id` int(11) NOT NULL,
  `username` varchar(50) NOT NULL,
  `password` varchar(255) NOT NULL,
  `name` varchar(500) NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `user`
--

INSERT INTO `user` (`id`, `username`, `password`, `name`, `created_at`) VALUES
(19, 'abid123', 'abid123', 'abid', '2024-05-15 05:46:04'),
(20, 'rahadityaputra', 'adit1234', 'Rahaditya Abimanyu ', '2024-05-21 13:59:27'),
(21, 'alfonsus', 'alfon123', 'Alfonsus Sitanggang', '2024-05-21 14:27:15');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `task`
--
ALTER TABLE `task`
  ADD PRIMARY KEY (`task_id`),
  ADD KEY `fk_user` (`user_id`);

--
-- Indexes for table `user`
--
ALTER TABLE `user`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `username` (`username`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `task`
--
ALTER TABLE `task`
  MODIFY `task_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=19;

--
-- AUTO_INCREMENT for table `user`
--
ALTER TABLE `user`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=22;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `task`
--
ALTER TABLE `task`
  ADD CONSTRAINT `fk_user` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
