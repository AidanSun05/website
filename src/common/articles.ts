import type { ImageMetadata } from "astro";
import { getCollection } from "astro:content";

const images = import.meta.glob<{ default: ImageMetadata }>("/src/img/*/*.{jpg,png,gif}");

export function formatDate(date: Date) {
  const monthNames = [
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
  ];
  return `${monthNames[date.getUTCMonth()]} ${date.getUTCDate()}, ${date.getUTCFullYear()}`;
}

export async function loadImage(image: string) {
  return (await images[`/src/img/${image}`]()).default;
}

export async function getArticlesSorted() {
  const articles = await getCollection("articles");
  articles.sort((a, b) => b.data.date.getTime() - a.data.date.getTime());
  return articles;
}
