import { getCollection } from "astro:content";

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

export async function getArticlesSorted() {
  const articles = await getCollection("articles");
  articles.sort((a, b) => b.data.revdate.getTime() - a.data.revdate.getTime());
  return articles;
}

export async function getAllTags() {
  const articles = await getCollection("articles");
  return articles.map(({ data }) => data.tags.split(" ")).flat();
}
