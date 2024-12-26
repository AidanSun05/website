<template>
  <div ref="slides" class="slides-container">
    <div class="slides-view">
      <div class="slides" :style="{ left: `-${slideNumber * slidesWidth}px` }">
        <div v-for="[i, [src, alt]] of images.entries()" :key="i" :style="{ width: `${slidesWidth}px` }">
          <img :src :alt />
        </div>
      </div>
    </div>
    <button v-show="slideNumber > 0" type="button" class="button-prev" @click="slideNumber--">&lt;</button>
    <button v-show="slideNumber < images.length - 1" type="button" class="button-next" @click="slideNumber++">
      &gt;
    </button>
    <div class="dots-container">
      <div
        v-for="i in images.length"
        :key="i"
        :class="{ dot: true, active: slideNumber === i - 1 }"
        @click="slideNumber = i - 1"
      ></div>
    </div>
    <p>{{ images[slideNumber][1] }}</p>
  </div>
</template>

<script setup lang="ts">
import { onMounted, ref, useTemplateRef } from "vue";

const props = defineProps<{
  width: number;
  images: string[][];
}>();

let slidesWidth = ref(0);
let slideNumber = ref(0);
const width = `${props.width}px`;

const slides = useTemplateRef("slides");
onMounted(() => {
  const el = slides.value;
  if (el === null) return;

  const resizeObserver = new ResizeObserver(() => (slidesWidth.value = el.getBoundingClientRect().width));
  resizeObserver.observe(el);
});
</script>

<style lang="postcss">
.slides-container {
  width: min(v-bind(width), 90vw);
  height: min-content;
  overflow: hidden;
  display: grid;
  grid-template-columns: auto 1fr auto;
  grid-template-rows: 1fr 1fr 1fr auto;

  & .slides-view {
    grid-column: 1 / 4;
    grid-row: 1 / 4;
  }

  & .button-prev,
  & .button-next {
    grid-row: 2;
    z-index: 9;
    transition: 0.2s;
    border: 0;
    background-color: #ddd;
    cursor: pointer;

    @media (pointer: fine) {
      opacity: 0;
    }
  }

  & .button-prev {
    grid-column: 1;
  }

  & .button-next {
    grid-column: 3;
  }

  &:hover .button-prev,
  &:hover .button-next {
    opacity: 1;
  }

  & .button-prev:hover,
  & .button-next:hover {
    background-color: #777;
  }

  & .dots-container {
    grid-column: 1 / 4;
    grid-row: 4;
    display: flex;
    flex-direction: row;
    justify-content: center;
    gap: 6px;
    flex-wrap: wrap;

    & .dot {
      width: 14px;
      height: 14px;
      border: 1px solid;
      transition: 0.2s;
      cursor: pointer;

      &:hover {
        background-color: #ddd;
      }
    }

    & .active {
      background-color: #777;
    }
  }

  & p {
    grid-column: 1 / 4;
    grid-row: 5;
    text-align: center;
  }

  & .slides {
    position: relative;
    transition: 0.2s;
    white-space: nowrap;
    width: max-content;

    display: flex;
    margin: 0;
  }
}
</style>
