angular.module('codepuppy').directive('submissionFileHolder', function() {
  return {
    restrict: 'E',

    scope: {
      'file': '=file',
      'commentsReleased': '=commentsreleased', 
      'commentPermissions': '=commentpermissions'
    },

    link: function($scope) {
     $scope.$watch('comments', function() {
      $scope.$$childHead.getComments();
     });
    },

    templateUrl: '/assets/partials/submissionFileHolder/submissionFileHolder.html'
  };
});