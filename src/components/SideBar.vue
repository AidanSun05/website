<template>
  <aside id="sidebar" :class="{ toggled: sidebarOpened }">
    <div id="searchbar">
      <input id="search-box" v-model.trim="searchQuery" type="text" placeholder="Search" />
      <button id="search-button" type="button" @click="doSearch"></button>
    </div>
    <nav id="sidebar-content">
      <template v-if="searchActive">
        <strong v-if="searchMatches.length === 0">No search results found.</strong>
        <strong v-else>Found {{ searchMatches.length }} search results.</strong>
      </template>
      <a href="/">Home</a>
      <a href="/about/">About</a>
      <button type="button" @click="articlesOpened = !articlesOpened">{{ articlesOpened ? "-" : "+" }}</button>
      <a href="/articles/">Articles</a>
      <template v-if="articlesOpened">
        <a
          v-for="page of articles as Page[]"
          :key="page.id"
          :href="`/articles/${page.id}/`"
          :class="{ highlight: searchActive && searchMatches.includes(page) }"
          >{{ page.data.doctitle }}</a
        >
      </template>
    </nav>
  </aside>
  <button id="menu-button" type="button" :class="{ toggled: sidebarOpened }" @click="sidebarOpened = !sidebarOpened">
    <div id="menu-button-arrow"></div>
  </button>
</template>

<script setup lang="ts">
import SearchIcon from "@img/search.svg";
import { ref, watch } from "vue";

// Manually typing Page interface to avoid importing dependency from Astro
interface Page {
  id: string;
  data: {
    title: string;
  };
}

const props = defineProps<{
  articles: unknown[];
}>();

let searchActive = ref(false);
let searchMatches = ref<unknown[]>([]);
const searchQuery = ref("");

let sidebarOpened = ref(false);
let articlesOpened = ref(true);
const searchIcon = `url(${SearchIcon.src})`;

function doSearch() {
  if (searchQuery.value.length === 0) {
    searchActive.value = false;
    return;
  }

  const fuse = new Fuse(props.articles, {
    keys: ["data.title", "data.description", "data.tags"],
    includeMatches: true,
    minMatchCharLength: 2,
    threshold: 0.5
  });

  searchActive = true;
  searchMatches = fuse.search(searchQuery).map((i) => i.item);
}

watch(searchQuery, () => setTimeout(doSearch, 500));
</script>

<style lang="postcss">
#searchbar {
  display: grid;
  grid-template-columns: auto 40px;
  border: none;
  border-bottom: 4px solid white;
}

#search-box {
  border: none;
  font-size: inherit;
  padding: 4px;
  color: white;
  background-color: #5a5a5a;
}

#search-button {
  border: none;
  background: #333 v-bind(searchIcon) center/16px 16px no-repeat;
  cursor: pointer;
}

::placeholder {
  color: #ddd;
  opacity: 1;
}

#sidebar {
  width: 300px;
  height: 100%;
  display: grid;
  grid-template-rows: 40px auto;
  position: fixed;
  top: 0;
  left: 0;
  background-color: #ddd;
  overflow-y: auto;
  z-index: 2;
  transition: 0.6s;

  transform: translateX(-300px);

  &.toggled {
    transform: translateX(0);
  }

  @media screen and (min-width: 1600px) {
    transform: translateX(0) !important;

    &.toggled {
      transform: translateX(-300px) !important;
    }
  }
}

#sidebar-content {
  overflow: auto;
  padding: 4px;
  display: grid;
  grid-template-columns: 1em auto;
  grid-auto-rows: max-content;
  gap: 4px 2px;
  align-items: center;

  strong {
    grid-column: 1 / span 2;
  }

  button {
    font-size: 100%;
    height: 1em;
    padding: 0;
    line-height: 0.5em;
    border: 1px solid;
    background-color: #eee;
    cursor: pointer;
  }

  a {
    grid-column: 2;
    padding: 4px;
    transition: 0.2s;
    text-decoration: none;
    color: black;

    &:hover {
      background-color: #aaa;
    }

    &.highlight {
      background-color: goldenrod;
    }
  }
}

#menu-button {
  width: 30px;
  height: 50px;
  border: 0;
  cursor: pointer;
  position: sticky;
  top: 0;
  background-color: #555;
  transition: 0.6s;

  transform: translateX(0);

  &.toggled {
    transform: translateX(300px);
  }

  @media screen and (min-width: 1600px) {
    transform: translateX(300px) !important;

    &.toggled {
      transform: translateX(0) !important;
    }
  }
}

#menu-button-arrow {
  width: 0;
  height: 0;
  border-top: 8px solid transparent;
  border-bottom: 8px solid transparent;
  border-left: 8px solid #ddd;
  margin: auto;
  transition: 0.6s;

  transform: rotateZ(0);

  #menu-button.toggled & {
    transform: rotateZ(0.5turn);
  }

  @media screen and (min-width: 1600px) {
    transform: rotateZ(0.5turn) !important;

    #menu-button.toggled & {
      transform: rotateZ(0) !important;
    }
  }
}
</style>
