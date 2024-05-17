-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: May 17, 2024 at 09:20 AM
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
  `deadline` date NOT NULL,
  `created_at` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

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
(1, 'rahadiyaputra', 'rahaditya123', 'rahaditya abimanyu putra', '2024-05-10 07:07:40'),
(2, 'uhui123', 'awokwowk123', '', '2024-05-10 14:08:37'),
(3, 'AWOK', '1234', '', '0000-00-00 00:00:00'),
(6, 'budi', '123', '', '2024-05-10 15:22:15'),
(7, 'a', 'h', '', '2024-05-10 15:30:41'),
(8, 'rahaditya', 'buku123', '', '2024-05-11 03:07:13'),
(9, 'df', 'fa', '', '2024-05-11 03:09:41'),
(10, 'bayu123', 'bayu_123', '', '2024-05-11 03:23:56'),
(11, 'bimo', 'bimo123', '', '2024-05-11 03:25:15'),
(13, 'adit', 'adit123', '', '2024-05-11 07:47:18'),
(14, 'ferdian123', 'ferdi123', 'ferdian', '2024-05-12 14:31:51'),
(16, 'upil_12', 'a12', 'bambang', '2024-05-12 14:34:28'),
(17, 'alfon123', 'alfon123', 'alfonsus', '2024-05-15 04:07:44'),
(18, '1', '1', '1', '2024-05-15 04:14:12'),
(19, 'abid123', 'abid123', 'abid', '2024-05-15 05:46:04');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `task`
--
ALTER TABLE `task`
  ADD PRIMARY KEY (`task_id`);

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
  MODIFY `task_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `user`
--
ALTER TABLE `user`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=20;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
