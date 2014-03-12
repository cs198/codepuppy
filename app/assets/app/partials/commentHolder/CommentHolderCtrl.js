angular.module('codepuppy').controller('CommentHolderCtrl',
  function($scope, $modal) {
      $scope.onClicked = function($event) {
        if($scope.commentPermissions) {
          var commentModal = $modal.open({
            templateUrl: '/assets/partials/commentModal/commentModal.html',
            controller: CommentModalCtrl,
            resolve: {
              commentBody: function()  {
                return $scope.commentBody;
              }
            }
          });

          commentModal.result.then(function(comment) {
            // Make a PUT request
            $scope.commentBody.comment = comment;
          });

          if ($event.stopPropagation) $event.stopPropagation();
          if ($event.preventDefault) $event.preventDefault();
          $event.cancelBubble = true;
          $event.returnValue = false;
        }
    };
  }
);
