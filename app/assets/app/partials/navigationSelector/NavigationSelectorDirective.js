angular.module('codepuppy').directive('navigationSelector', function() {
  return {
    restrict: 'E',
    scope: {
      'title': '=title',
      'elements': '=elements'
    },
    templateUrl: '/assets/partials/navigationSelector/navigationSelector.html'
  };
});
