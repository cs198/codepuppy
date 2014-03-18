angular.module('codepuppy').directive('submissionFileHolder', ['$timeout', function($timeout) {
  return {
    restrict: 'E',

    scope: {
      'file': '=file',
      'commentsReleased': '=commentsreleased', 
      'commentPermissions': '=commentpermissions'
    },

    link: function($scope) {
      var highlight = function() {
        $scope.rendered = true;
        Prism.highlightAll();
      };
      $timeout(highlight, 1000);
     $scope.$watch('comments', function() {
      $scope.$$childHead.getComments();
     });
    },

    templateUrl: '/assets/partials/submissionFileHolder/submissionFileHolder.html'
  };
}]);